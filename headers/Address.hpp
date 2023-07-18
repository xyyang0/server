#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
struct Address{
    Address();
    Address(const char *ip,const char *port);
    ~Address();
    struct sockaddr_in addr;
    int len;
};