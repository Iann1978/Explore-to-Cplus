#include <iostream>
#include <future>
#include <coroutine>
#include <chrono>
#include <thread>

struct ReturnObject5 {
    struct promise_type {


        ~promise_type() {
            std::cout << "promise_type destroyed" << std::endl;
        }
        ReturnObject5 get_return_object() {
            return {
              .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}

        void return_void() {}
    };

    std::coroutine_handle<promise_type> h_;
};

ReturnObject5
counter5()
{
    //for (unsigned i = 0; i < 3; ++i)
        //co_yield unsigned(0);
        //co_yield unsigned(1);
        //co_yield unsigned(2);
    // falling off end of function or co_return; => promise.return_void();
    // (co_return value; => promise.return_value(value);)

    co_await std::suspend_always{};
    co_await std::suspend_always{};
    co_await std::suspend_always{};
}

void
main()
{
    auto h = counter5().h_;
    auto& promise = h.promise();
    while (!h.done()) { // Do NOT use while(h) (which checks h non-NULL)
       // std::cout << "counter5: " << promise.value_ << std::endl;
        h();
    }
    h.destroy();
}