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

void thread_poll::push(const TaskFunction& function) {
    if (!this->started.load()) {
        throw "Thread poll not started.";
    }

    {
        lock_guard<mutex> lock{this->sync_lock};
        this->task_queue.emplace(function);
    }

    this->condition.notify_one();
}