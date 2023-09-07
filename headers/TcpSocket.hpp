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
#include "Address.hpp"
#include <memory>
class TcpSocket{
public:
    TcpSocket();
    ~TcpSocket();

    void bind(const Address *addr)const;
    void listen()const;
    std::tuple<int,Address> accept()const;
    int connect(Address *addr);

    int getFd()const;
private:
    int sockfd;
};
#endif