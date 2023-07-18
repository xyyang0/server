#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include "../headers/Address.hpp"
#include "../headers/TcpSocket.hpp"
#define MAX_BUF 1024

int main(int argc,char **argv){
    if(argc < 3){
        std::cerr << "./client [ip] [port]";
        exit(-1);
    }
    TcpSocket clientSock;
    Address servAddr(argv[1],argv[2]);
    clientSock.connect(servAddr);

    char buf[MAX_BUF]{0};
    while(1){
        write(STDOUT_FILENO,"<in>",4);
        int ret = read(STDIN_FILENO,buf,1024);
        if(ret < 0){
         perror(strerror(errno));
         exit(-1);           
        }else if(ret == 0){
            break;
        }else{
            write(clientSock.getFd(),buf,strlen(buf));
        }
        ret = read(clientSock.getFd(),buf,1024);
        write(STDOUT_FILENO,"<out>",5);
        write(STDOUT_FILENO,buf,5);
        write(STDOUT_FILENO,"\n",strlen("\n"));
    }
}