#ifndef EPOLL_H_
#define EPOLL_H_

#include <iostream>
#include <list>
#include <unistd.h>
#include <sys/epoll.h>
#include "Util.hpp"
#include "TcpSocket.hpp"
#include "threadpool.hpp"
#include <vector>
class Epoll{
public:
    Epoll();
    ~Epoll();
    void addFd(int fd,int flags);
    void modfd(int fd,int flags);
    int getEpollFd();
    std::vector<epoll_event> eventLoop();
private:
    enum { MAX_EVENTS = 1024 };
    enum { MAX_BUF = 256 };
    int epollfd;
    struct epoll_event events[MAX_EVENTS];
};
#endif