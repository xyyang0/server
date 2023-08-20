#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
struct Address{
    Address();
    Address(const std::string& ip,const std::string& port);
    ~Address();
    struct sockaddr_in addr;
    int len;
};