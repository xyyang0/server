#include "Epoll.hpp"
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

void Epoll::addFd(int fd,int flags){
    setnonblock(fd);
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = flags;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void Epoll::eventLoop(const ServerAddr &servaddr){
    while(1){
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == servaddr.getFd()){
                int acceptfd = servaddr.accept();
                if(acceptfd > 0){
                    setnonblock(acceptfd);
                    addFd(acceptfd, EPOLLIN | EPOLLET);
                }
            }else if(events[i].events | EPOLLIN){
                readWriteEvents(events[i].data.fd);
            }
        }
    }
}

void Epoll::readWriteEvents(int clientfd){
    while(1){
        char buf[MAX_BUF];
        bzero(buf,sizeof(buf));
        int ret = read(clientfd, buf, MAX_BUF);
        if(ret < 0 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
            close(clientfd);
            break;
        }else if(ret < 0 && errno == EINTR){
            continue;
        }else if(ret == 0){
            close(clientfd);
            break;
        }else{
            write(clientfd,buf,strlen(buf));
            break;
        }
    }
}