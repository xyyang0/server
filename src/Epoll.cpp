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

void Epoll::addFd(int fd,int flags){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = flags;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}


void readWriteEvents(http *ht){
    ht->process_routine();
}

void Epoll::eventLoop(const TcpSocket &servSock){
    while(1){
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == servSock.getFd()){
                int acceptfd = servSock.accept();
                if(acceptfd > 0){
                    setnonblock(acceptfd);
                    addFd(acceptfd, EPOLLIN | EPOLLET);
                }
            }else if(events[i].events | EPOLLIN){
                http* ht = nullptr;
                for(http *item : http::httpConns){
                    if(item->getFd() == events[i].data.fd){
                        ht = item;
                        break;
                    }
                    else if(!item->connState()){
                        ht = item;
                        ht->setFd(events[i].data.fd);
                        break;
                    }
                }
                if(!ht){
                    ht = new http{events[i].data.fd};
                    http::httpConns.insert(ht);
                }
                thPool.addTask(readWriteEvents,ht);
            }
        }
    }
}
