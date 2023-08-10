#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
class ThreadPool{
public:
    ThreadPool(int num = std::thread::hardware_concurrency());
    ~ThreadPool();
    template <typename F,typename... Args>
    auto addTask(F&& f,Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex taskMtx;
    std::condition_variable cond;
    bool stop{false};

    unsigned int maxThreadsNum{ std::thread::hardware_concurrency() };
};

template <typename F,typename... Args>
auto ThreadPool::addTask(F&& f,Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f),std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        if(stop){
            throw std::runtime_error("enqueue on stoped Threadpool");
        }
        tasks.emplace( [task](){ (*task)(); } );
    }
    cond.notify_one();
    return res;
}
