#include <iostream>
#include <future>
#include <coroutine>
#include <chrono>
#include <thread>
//
//// Awaiter for std::future
//template<typename T>
//struct FutureAwaiter {
//    std::future<T>& future;
//
//    bool await_ready() const noexcept {
//        return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
//    }
//
//    T await_resume() {
//        return future.get();
//    }
//
//    void await_suspend(std::coroutine_handle<> h) {
//        // Launch a thread to wait for the future
//        std::thread([this, h]() {
//            future.wait();
//            h.resume();
//            }).detach();
//    }
//};
//
//// Helper function to make future awaitable
//template<typename T>
//FutureAwaiter<T> operator co_await(std::future<T>& future) {
//    return FutureAwaiter<T>{future};
//}
//
//// Coroutine task type
//template<typename T>
//struct Task {
//    struct promise_type {
//        T value;
//        std::exception_ptr exception;
//
//        Task get_return_object() {
//            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
//        }
//
//        std::suspend_never initial_suspend() { return {}; }
//        std::suspend_never final_suspend() noexcept { return {}; }
//
//        void return_value(T val) {
//            value = val;
//        }
//
//        void unhandled_exception() {
//            exception = std::current_exception();
//        }
//    };
//
//    std::coroutine_handle<promise_type> coro;
//
//    Task(std::coroutine_handle<promise_type> h) : coro(h) {}
//
//    ~Task() {
//        if (coro) coro.destroy();
//    }
//
//    T get() {
//        if (coro.promise().exception) {
//            std::rethrow_exception(coro.promise().exception);
//        }
//        return coro.promise().value;
//    }
//};
//
//// Demo functions
//std::future<int> async_compute(int value) {
//    return std::async([value]() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        return value * 2;
//        });
//}
//
//std::future<std::string> async_string(const std::string& input) {
//    return std::async([input]() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//        return "Processed: " + input;
//        });
//}
//
//// Coroutine that awaits futures
//Task<int> demo_coroutine() {
//    std::cout << "Starting coroutine..." << std::endl;
//
//    // Await multiple futures
//    auto future1 = async_compute(10);
//    auto future2 = async_compute(20);
//    auto future3 = async_string("Hello");
//
//    std::cout << "Created futures, awaiting..." << std::endl;
//
//    int result1 = co_await future1;
//    int result2 = co_await future2;
//    std::string result3 = co_await future3;
//
//    std::cout << "Results: " << result1 << ", " << result2 << ", " << result3 << std::endl;
//
//    co_return result1 + result2;
//}
//
//// Main function
//int main() {
//    std::cout << "Starting demo..." << std::endl;
//
//    auto task = demo_coroutine();
//    int final_result = task.get();
//
//    std::cout << "Final result: " << final_result << std::endl;
//
//    return 0;
//}
struct Task {
	struct promise_type {

		std::exception_ptr exception;

		Task get_return_object() {
			std::cout << "create task." << std::endl;
			return {
				.h_ = std::coroutine_handle<promise_type>::from_promise(*this)
			};
		}

		std::suspend_never initial_suspend() {
			return {};
		}
		std::suspend_never final_suspend() noexcept {
			return {};
		}

		void return_void()
		{
		}

		void unhandled_exception() {
			exception = std::current_exception();
		}
	};



	std::coroutine_handle<promise_type> h_;
	operator std::coroutine_handle<promise_type>() const
	{
		std::cout << "convert tast to handle" << std::endl;
		return h_; 
	}
	operator std::coroutine_handle<>() const
	{
		std::cout << "convert tast to generic handle" << std::endl;
		return h_;
	}
};

struct ReturnObject2 {
	struct promise_type {
		ReturnObject2 get_return_object() {
			return {
				// Uses C++20 designated initializer syntax
				.h_ = std::coroutine_handle<promise_type>::from_promise(*this)
			};
		}
		void return_void()
		{
			int a = 0;
			int b = 0;
		}
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void unhandled_exception() {}
	};

	std::coroutine_handle<promise_type> h_;
};

struct ReturnObject5 {
	struct promise_type {
		ReturnObject5 get_return_object() {
			return {
			  .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
			};
		}
		void return_void()
		{
			int a = 0;
			int b = 0;
		}
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void unhandled_exception() {}
	};

	std::coroutine_handle<promise_type> h_;
};


class Shader
{
public:
	std::string name;
	std::string context;
	std::string parsingResult;
public:
	Shader(std::string name) : name{ name } {};

	std::string load(std::string filename)
	{
		std::cout << "loading " << filename << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "loaded  " << filename << std::endl;
		std::string context = "context of file " + filename;
		return context;
	}

	std::string parse(std::string context)
	{
		std::cout << "parsing " << context << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "parsed  " << context << std::endl;
		std::string parsingResult = "result of pasing " + context;
		return parsingResult;
	}

	std::future<std::string> async_load(std::string filename)
	{
		auto future = std::async([this,filename]() {
			return this->load(filename);
		});
		return future;
	}

	std::future<std::string> async_parse(std::string filename)
	{
		auto future = std::async([this, filename]() {
			return this->parse(filename);
			});
		return future;
	}

	static Shader* LoadAndCreate(std::string filename)
	{
		Shader* shader = new Shader(filename);
		shader->context = shader->load(filename);
		shader->parsingResult = shader->parse(filename);
		return shader;
	}

	static ReturnObject5 CoLoadAndCreate(std::string filename)
	{


		std::cout << "enter CoLoadAndCreate" << std::endl;
		std::cout << "waiting a" << std::endl;
		co_await std::suspend_always{};
		std::cout << "waited a" << std::endl;
		co_await std::suspend_always{};
		std::cout << "waited b" << std::endl;
		co_return;
	}
};

int main()
{
	std::cout << "hello coroutine." << std::endl;

	//Shader* shader0 = Shader::LoadAndCreate("shader0");
	//Shader* shader1 = Shader::LoadAndCreate("shader1");

	std::coroutine_handle<> h = Shader::CoLoadAndCreate("shader0").h_;
	
	//for (int i = 0; i < 3; ++i) {
	//	std::cout << "In main2 function\n";
	//	h();
	//}
	while (!h.done())
	{
		int a = 0;
		int b = 0;
		h.resume();
		int c = 0;
		int d = 0;
	}

	return 0;
}