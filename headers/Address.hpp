#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <tuple>
class Address{
public:
    Address();
    Address(const std::string& ip,const std::string& port);
    Address(const struct sockaddr_in &addr_st,socklen_t l);
    ~Address();

    std::string get_ip()const;
    std::string get_port()const;
    struct sockaddr_in get_trivial_addr()const;
    socklen_t get_addr_len()const;
private:
    struct sockaddr_in addr;
    socklen_t len;
};