#pragma once
#include <unistd.h>
#include <string>
#include <fstream>
#include <sys/mman.h>
#include <sys/uio.h>
#include "buffer.hpp"
#include "timer.hpp"
#include "Util.hpp"
#include "objectPool.hpp"
#include "timer.hpp"
#include "Epoll.hpp"
#include <iostream>
#include <set>
class http{
public:
    http()=default;
    ~http()=default;

    int readn();
    int writen();

    void process_request();
    void process_response();

    void process(const std::vector<std::string> &inputStrs);
    void parse_request_line(const std::vector<std::string> &inputStrs);
    void parse_request_header(const std::vector<std::string> &inputStrs);
    void parse_content(const std::vector<std::string> &inputStrs);

    int check_file();

    void add_response();
    void add_response_line(const char *str);
    void add_response_header();
    void add_blank_line();
    void fill_packet();

    void process_write();

    int getFd();
    void setHttp(timer *tp,Epoll *p,int fd);
    void reset();
    void httpClose();

    static void httpProcessRead(Object<http> *);
    static void httpProcessWrite(Object<http> *);

    timer *t{nullptr};
    Epoll *Ep{nullptr};
private:
    enum class http_method { GET,POST } ;
    enum class http_state { OK = 200,BAD_REQUEST = 404 };
    enum { MAX_CLIENTS = 65535 };
    int clientfd;

    std::string version;
    std::string url;
    http_method method;
    http_state state;
    std::string content_type;

    unsigned int content_length{0};
    bool keep_alive{false};
    int current_line{0};

    void *mapaddr{nullptr};
    Buffer readBuf;
    Buffer writeBuf;
};