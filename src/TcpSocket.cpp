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
    struct sockaddr_in adr = addr->get_trivial_addr();
    socklen_t len = addr->get_addr_len();
    int ret = ::bind(sockfd, (struct sockaddr*)(&adr),len);
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

std::tuple<int,Address> TcpSocket::accept()const{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int fd = ::accept(sockfd, (struct sockaddr *)(&addr), &len);
    if(fd < 0){
       perror("accept:"); 
    }
    return {fd,Address{addr,len}};
}

int TcpSocket::connect(Address *addr){
    struct sockaddr_in adr = addr->get_trivial_addr();
    socklen_t len = addr->get_addr_len();
    return ::connect(sockfd,(struct sockaddr*)(&adr.sin_addr),len);
}

int TcpSocket::getFd()const{
    return sockfd;
}
TcpSocket::~TcpSocket(){
    close(sockfd);
}
