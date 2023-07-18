#ifndef SERVERADDR_H_
#define SERVERADDR_H_

#include <iostream>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/epoll.h>

class ServerAddr{
public:
    ServerAddr();
    ~ServerAddr();

    void bind()const;
    void listen()const;
    int accept()const;

    int getFd()const;
private:
    int sockfd;
};
#endif