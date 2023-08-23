#include "../headers/Epoll.hpp"
#include <memory>
#include <sys/epoll.h>
Epoll::Epoll(){
    epollfd = epoll_create(256);
    if(epollfd < 0){
        perror("epoll:");
        exit(-1);
    }
}

Epoll::~Epoll(){
    close(epollfd);
}

Epoll* Epoll::getInstance(){
    static Epoll Ep;
    return &Ep;
}

void Epoll::addFd(int fd,int flags){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = flags;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void Epoll::modfd(int fd, int flags){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = flags;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

std::vector<epoll_event> Epoll::poll(){
    std::vector<epoll_event> pollEvents;
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        for(int i = 0; i < nfds; i++){
            pollEvents.push_back(events[i]);
        }
        return pollEvents;
}