#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "./headers/Epoll.hpp"
#include "./headers/Address.hpp"
#include "./headers/TcpSocket.hpp"
int main(int argc,char **argv){
    if(argc < 3){
        std::cerr << "./server [ip] [port]";
        exit(-1);
    }
    TcpSocket servSock;
    Address servaddr(argv[1],argv[2]);
    servSock.bind(servaddr);
    servSock.listen();

    Epoll Ep;
    Ep.addFd(servSock.getFd(),EPOLLIN);
    Ep.eventLoop(servSock);
}
