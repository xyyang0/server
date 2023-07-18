#ifndef EPOLL_H_
#define EPOLL_H_

#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include "Util.hpp"
#include "TcpSocket.hpp"
class Epoll{
public:
    Epoll();
    ~Epoll();
    void addFd(int fd,int flags);
    void eventLoop(const TcpSocket &servSock);
private:
    enum { MAX_EVENTS = 1024 };
    enum { MAX_BUF = 256 };
    int epollfd;
    struct epoll_event events[MAX_EVENTS];

    void readWriteEvents(int clinetfd);
};
#endif