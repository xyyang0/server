#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include "Epoll.hpp"

class http;
struct timer{
    timer(uint64_t expire,http *h,Epoll *ep):m_expire(expire),hptr(h),Ep(ep){}
    ~timer(){};

    template <typename F,typename ...Args>
    void callback(F &&f,Args&& ...args){
        func = std::bind(std::forward<F>(f),std::forward<Args>(args)...);
    }
    std::function<void()> func;
    uint64_t m_expire;
    http *hptr;
    Epoll *Ep;
};
