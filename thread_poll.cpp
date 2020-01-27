#include "thread_poll.h"

thread_poll::thread_poll(int count) {
    this->poll_count.store(count);
    this->started.store(true);

    for (int i = 0; i < count; i++) {
        this->poll.emplace_back(
            [this]() -> void {
                while (this->started.load()) {
                    function<void()> func;
                    {
                        unique_lock<mutex> lock{this->sync_lock};
                        this->condition.wait(lock, [this] {
                            return (!this->started.load()) || (!this->task_queue.empty());
                        });

                        if (!this->started.load() || this->task_queue.empty()) return;
                        func = move(this->task_queue.front());
                        this->task_queue.pop();
                    }
                    func();
                }
            }
        );
    }
}

thread_poll::~thread_poll() {
    this->started.store(false);
    this->condition.notify_all();

    for (thread& _thread : this->poll) {
        if (_thread.joinable()) _thread.join();
    }
}

template<class F, class... Args>
auto thread_poll::push(F&& f, Args&&... args) -> future<decltype(f(args...))> {
    if (!this->started.load()) {
        throw "Thread poll not started.";
    }

    using ReturnType = decltype(f(args...));
    auto wrapped_function = make_shared<packaged_task<ReturnType()>>(
        bind(forward<F>(f), forward<Args...>(args)...)
    );

    future<ReturnType> final_future = wrapped_function->get_future();
    {
        lock_guard<mutex> lock{this->sync_lock};
        this->task_queue.emplace([wrapped_function]() {
            (*wrapped_function)();
        });
    }

    this->condition.notify_one();
    return final_future;
}