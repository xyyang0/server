#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include "Epoll.hpp"

class http;
struct timer{
    timer(uint64_t expire,http *ptr):m_expire(expire),h(ptr){}
    ~timer()=default;

    template <typename F,typename ...Args>
    void callback(F &&f,Args&& ...args){
        func = std::bind(std::forward<F>(f),std::forward<Args>(args)...);
    }
    std::function<void()> func;
    uint64_t m_expire;
    http *h{nullptr};
    bool keep_alive{true};
};
