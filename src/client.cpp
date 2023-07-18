#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror(strerror(errno));
        exit(-1);
    }

    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret;
    ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0){
        perror(strerror(errno));
        exit(-1);
    }

    char buf[1024]{0};
    while(1){
        write(STDOUT_FILENO,"<in>",4);
        ret = read(STDIN_FILENO,buf,1024);
        if(ret < 0){
         perror(strerror(errno));
         exit(-1);           
        }else if(ret == 0){
            break;
        }else{
            write(sockfd,buf,strlen(buf));
        }
        ret = read(sockfd,buf,1024);
        write(STDOUT_FILENO,"<out>",5);
        write(STDOUT_FILENO,buf,5);
        write(STDOUT_FILENO,"\n",strlen("\n"));
    }
    close(sockfd);
}