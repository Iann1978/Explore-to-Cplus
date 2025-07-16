#include <iostream>
#include <future>
#include <coroutine>
#include <chrono>
#include <thread>

// Awaiter for std::future - Solution 2: Store by value
template<typename T>
struct FutureAwaiter {
    std::future<T> future;  // Value, not reference

    // Constructor takes future by value and moves it
    FutureAwaiter(std::future<T> f) : future(std::move(f)) {
        std::cout << "FutureAwaiter constructed with future (value)" << std::endl;
    }

    bool await_ready() const noexcept {
        std::cout << "await_ready() called - checking if future is ready" << std::endl;
        return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    T await_resume() {
        std::cout << "await_resume() called - getting value from future" << std::endl;
        return future.get();
    }

    void await_suspend(std::coroutine_handle<> h) {
        std::cout << "await_suspend() called - future not ready, launching thread" << std::endl;
        // Launch a thread to wait for the future
        std::thread([this, h]() {
            std::cout << "Thread started - waiting for future" << std::endl;
            future.wait();
            std::cout << "Future ready - resuming coroutine" << std::endl;
            h.resume();
        }).detach();
    }
};

// Helper function to make future awaitable
// Now works with both lvalue and rvalue futures
template<typename T>
FutureAwaiter<T> operator co_await(std::future<T> future) {
    std::cout << "operator co_await called with future (value)" << std::endl;
    return FutureAwaiter<T>{std::move(future)};
}

// Coroutine task type
template<typename T>
struct Task {
    struct promise_type {
        T value;
        std::exception_ptr exception;

        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }

        void return_value(T val) {
            value = val;
        }

        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> coro;

    Task(std::coroutine_handle<promise_type> h) : coro(h) {}

    ~Task() {
        if (coro) coro.destroy();
    }

    T get() {
        if (coro.promise().exception) {
            std::rethrow_exception(coro.promise().exception);
        }
        return coro.promise().value;
    }
};

// Demo function that creates a future
std::future<int> create_future(int value) {
    return std::async([value]() {
        std::cout << "Async task started with value: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Async task completed" << std::endl;
        return value * 2;
    });
}

// Coroutine that demonstrates the flow
Task<int> demo_coroutine() {
    std::cout << "\n=== Starting coroutine ===" << std::endl;
    
    // Create a future
    std::cout << "Creating future..." << std::endl;
    //auto xxxfuture = create_future(21);
    //std::cout << "Future created at address: " << &xxxfuture << std::endl;
    
    // This is where the magic happens!
    std::cout << "\n--- About to call co_await ---" << std::endl;
    int result = co_await create_future(21);
    std::cout << "--- co_await completed ---" << std::endl;
    
    std::cout << "Result: " << result << std::endl;
    co_return result;
}

// Coroutine that demonstrates Solution 2 works with temporary futures
Task<int> demo_solution2_coroutine() {
    std::cout << "\n=== Solution 2 Demo: Temporary Future ===" << std::endl;
    
    // This now works with Solution 2!
    std::cout << "About to call co_await with temporary future..." << std::endl;
    int result = co_await create_future(42);
    std::cout << "co_await with temporary completed!" << std::endl;
    
    std::cout << "Result: " << result << std::endl;
    co_return result;
}

int main() {
    std::cout << "=== FutureAwaiter Demo - Solution 2 ===" << std::endl;
    
    // Test original approach (still works)
    //auto task1 = demo_coroutine();
    //int final_result1 = task1.get();
    //std::cout << "\nFinal result 1: " << final_result1 << std::endl;
    
    // Test new approach (now works with Solution 2!)
    auto task2 = demo_solution2_coroutine();
    int final_result2 = task2.get();
    std::cout << "\nFinal result 2: " << final_result2 << std::endl;
    
    std::cout << "\n=== Both approaches work with Solution 2! ===" << std::endl;
    
    return 0;
} 