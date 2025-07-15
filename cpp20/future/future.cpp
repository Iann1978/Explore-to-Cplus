// future.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <functional>
#include <random>

// Example 1: Basic async with std::future
int calculate_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    return x * x;
}

//// Example 2: Function that throws an exception
//int divide_numbers(int a, int b) {
//    if (b == 0) {
//        throw std::runtime_error("Division by zero!");
//    }
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    return a / b;
//}
//
//// Example 3: Function returning a complex type
//std::string generate_random_string(int length) {
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<> dis('a', 'z');
//
//    std::string result;
//    result.reserve(length);
//
//    for (int i = 0; i < length; ++i) {
//        result += static_cast<char>(dis(gen));
//    }
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(200));
//    return result;
//}
//
//// Example 4: Using std::packaged_task
//class TaskManager {
//public:
//    template<typename F, typename... Args>
//    auto submit_task(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
//        using return_type = decltype(f(args...));
//
//        auto task = std::packaged_task<return_type()>(
//            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
//        );
//
//        auto future = task.get_future();
//
//        // Store the task to keep it alive
//        tasks_.push_back(std::move(task));
//
//        // Execute in a separate thread
//        std::thread([this, task_index = tasks_.size() - 1]() {
//            tasks_[task_index]();
//            }).detach();
//
//        return future;
//    }
//
//private:
//    std::vector<std::packaged_task<void()>> tasks_;
//};
//
//// Example 5: Using std::promise and std::future
//class DataProcessor {
//public:
//    std::future<int> process_data_async(const std::vector<int>& data) {
//        std::promise<int> promise;
//        auto future = promise.get_future();
//
//        std::thread([this, data = data, promise = std::move(promise)]() mutable {
//            try {
//                int result = 0;
//                for (int value : data) {
//                    result += value;
//                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//                }
//                promise.set_value(result);
//            }
//            catch (...) {
//                promise.set_exception(std::current_exception());
//            }
//            }).detach();
//
//        return future;
//    }
//};
//
//// Example 6: Using std::shared_future for multiple consumers
//std::shared_future<std::string> create_shared_future() {
//    auto future = std::async(std::launch::async, []() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(300));
//        return std::string("Shared data from async task");
//        });
//
//    return future.share();
//}

int main() {
    std::cout << "=== C++20 std::future Demo ===\n\n";

    // Example 1: Basic async usage
    std::cout << "1. Basic async with std::future:\n";
    auto future1 = std::async(std::launch::async, calculate_square, 5);
    auto future2 = std::async(std::launch::async, calculate_square, 10);

    std::cout << "   Calculating squares asynchronously...\n";
    std::cout << "   5^2 = " << future1.get() << "\n";
    std::cout << "   10^2 = " << future2.get() << "\n\n";

    //// Example 2: Exception handling
    //std::cout << "2. Exception handling with std::future:\n";
    //auto future3 = std::async(std::launch::async, divide_numbers, 10, 2);
    //auto future4 = std::async(std::launch::async, divide_numbers, 10, 0);

    //try {
    //    std::cout << "   10 / 2 = " << future3.get() << "\n";
    //}
    //catch (const std::exception& e) {
    //    std::cout << "   Error: " << e.what() << "\n";
    //}

    //try {
    //    std::cout << "   10 / 0 = " << future4.get() << "\n";
    //}
    //catch (const std::exception& e) {
    //    std::cout << "   Error: " << e.what() << "\n";
    //}
    //std::cout << "\n";

    //// Example 3: Complex return types
    //std::cout << "3. Complex return types:\n";
    //auto future5 = std::async(std::launch::async, generate_random_string, 10);
    //auto future6 = std::async(std::launch::async, generate_random_string, 15);

    //std::cout << "   Generating random strings...\n";
    //std::cout << "   String 1: " << future5.get() << "\n";
    //std::cout << "   String 2: " << future6.get() << "\n\n";

    //// Example 4: Using std::packaged_task
    //std::cout << "4. Using std::packaged_task:\n";
    //TaskManager task_manager;

    //auto task_future1 = task_manager.submit_task(calculate_square, 7);
    //auto task_future2 = task_manager.submit_task(generate_random_string, 8);

    //std::cout << "   Submitting tasks to task manager...\n";
    //std::cout << "   Task 1 result: " << task_future1.get() << "\n";
    //std::cout << "   Task 2 result: " << task_future2.get() << "\n\n";

    //// Example 5: Using std::promise and std::future
    //std::cout << "5. Using std::promise and std::future:\n";
    //DataProcessor processor;
    //std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    //auto process_future = processor.process_data_async(data);
    //std::cout << "   Processing data asynchronously...\n";
    //std::cout << "   Sum of data: " << process_future.get() << "\n\n";

    //// Example 6: Using std::shared_future
    //std::cout << "6. Using std::shared_future for multiple consumers:\n";
    //auto shared_future = create_shared_future();

    //// Multiple threads can access the same result
    //auto consumer1 = std::async(std::launch::async, [&shared_future]() {
    //    return "Consumer 1: " + shared_future.get();
    //    });

    //auto consumer2 = std::async(std::launch::async, [&shared_future]() {
    //    return "Consumer 2: " + shared_future.get();
    //    });

    //std::cout << "   " << consumer1.get() << "\n";
    //std::cout << "   " << consumer2.get() << "\n\n";

    //// Example 7: Future status and timing
    //std::cout << "7. Future status and timing:\n";
    //auto future7 = std::async(std::launch::async, []() {
    //    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //    return 42;
    //    });

    //std::cout << "   Future status: ";
    //if (future7.wait_for(std::chrono::milliseconds(50)) == std::future_status::ready) {
    //    std::cout << "Ready\n";
    //}
    //else {
    //    std::cout << "Not ready yet\n";
    //}

    //std::cout << "   Final result: " << future7.get() << "\n\n";

    //// Example 8: Multiple futures with when_all (C++20 concept)
    //std::cout << "8. Multiple futures coordination:\n";
    //auto futures = std::vector<std::future<int>>();

    //for (int i = 1; i <= 5; ++i) {
    //    futures.push_back(std::async(std::launch::async, [i]() {
    //        std::this_thread::sleep_for(std::chrono::milliseconds(i * 50));
    //        return i * i;
    //        }));
    //}

    //std::cout << "   Waiting for all futures to complete...\n";
    //for (size_t i = 0; i < futures.size(); ++i) {
    //    std::cout << "   Future " << (i + 1) << ": " << futures[i].get() << "\n";
    //}

    //std::cout << "\n=== Demo completed successfully! ===\n";
    return 0;
}
