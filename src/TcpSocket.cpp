#include "../headers/TcpSocket.hpp"
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
TcpSocket::TcpSocket(){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("socket:");
        exit(-1);
    }
}
void TcpSocket::bind(const Address *addr)const{
    int ret = ::bind(sockfd, (struct sockaddr*)(&addr->addr), addr->len);
    if(ret < 0){
        perror("bind:");
        exit(-1);
    }
}

void TcpSocket::listen()const{
    if(::listen(sockfd,5) < 0){
        perror("listen:");
        exit(-1);
    }
}

int TcpSocket::accept()const{
    struct sockaddr_in clientAddr;
    bzero(&clientAddr,sizeof(clientAddr));
    socklen_t len = sizeof(clientAddr);
    int fd = ::accept(sockfd, (struct sockaddr *)&clientAddr, &len);
    if(fd < 0){
       perror("accept:"); 
    }
    return fd;
}

int TcpSocket::connect(Address *addr){
    return ::connect(sockfd,(struct sockaddr*)(&addr->addr),addr->len);
}

int TcpSocket::getFd()const{
    return sockfd;
}
TcpSocket::~TcpSocket(){
    close(sockfd);
}
