#pragma once

#include <unistd.h>
#include <string>
class http{
public:
    http(int fd):clientfd(fd){}
    ~http(){ close(clientfd); }

    int readn();
    int writen();

    void request();
    void response();

    void parse_header();
private:
    enum class http_method { HEAD,POST } ;
    enum class http_state { OK,FORBIDDEN,BAD_REQUEST };

    int clientfd;
    std::string url;
    http_method method;
    http_state state;
};