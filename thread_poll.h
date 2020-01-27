#ifndef WEB_SERVER_THREAD_POOL_H
#define WEB_SERVER_THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <memory>

using std::thread, std::mutex, std::condition_variable, std::atomic, std::lock_guard, std::unique_lock;
using std::future, std::packaged_task;
using std::vector, std::queue, std::bind, std::forward, std::function, std::make_shared;

class thread_poll {
private:
    using TaskFunction = std::function<void()>;

    vector<thread> poll;

    atomic<bool> started;
    atomic<int> poll_count;
public:
    mutex sync_lock;
    condition_variable condition;
    queue<TaskFunction> task_queue;

    thread_poll(int count = 8);
    ~thread_poll();

    template<class F, class... Args>
    auto push(F&& f, Args&&... args) -> future<decltype(f(args...))>;
};


#endif //WEB_SERVER_THREAD_POOL_H
