#include "../headers/threadpool.hpp"
#include <mutex>
#include <future>
#include <memory>
#include <stdexcept>

ThreadPool::ThreadPool(int num):threads(num){
    for(int i = 0; i < num; i++){
        threads.emplace_back([this](){
            while(1){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(taskMtx);
                    cond.wait(lock,[this](){
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(taskMtx);
        stop = true;
    }    
    cond.notify_all();
    for(auto &th : threads){
        if(th.joinable())
            th.join();
    }
}
