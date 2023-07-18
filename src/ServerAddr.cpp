#include "ServerAddr.hpp"
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
ServerAddr::ServerAddr(){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("socket:");
        exit(-1);
    }
}
void ServerAddr::bind()const{
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = PF_INET;
    addr.sin_port = htons(12345);

    int ret = ::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("listen:");
        exit(-1);
    }
}

void ServerAddr::listen()const{
    if(::listen(sockfd,5) < 0){
        perror("listen:");
        exit(-1);
    }
}

int ServerAddr::accept()const{
    struct sockaddr_in clientAddr;
    bzero(&clientAddr,sizeof(clientAddr));
    socklen_t len = sizeof(clientAddr);
    int fd = ::accept(sockfd, (struct sockaddr *)&clientAddr, &len);
    if(fd < 0){
       perror("accept:"); 
    }
    return fd;
}
int ServerAddr::getFd()const{
    return sockfd;
}
ServerAddr::~ServerAddr(){
    close(sockfd);
}
