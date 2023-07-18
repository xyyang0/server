#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "Epoll.hpp"
#include "ServerAddr.hpp"

int main(int argc,char **argv){
    ServerAddr servaddr;
    servaddr.bind();
    servaddr.listen();

    Epoll Ep;
    Ep.addFd(servaddr.getFd(),EPOLLIN);
    Ep.eventLoop(servaddr);
}
