#pragma once
#include <unistd.h>
#include <string>
#include <fstream>
#include <sys/mman.h>
#include <sys/uio.h>
#include "buffer.hpp"
#include "Util.hpp"
#include <iostream>
#include <set>
class http{
public:
    http(int fd):clientfd(fd){}
    ~http(){}

    int readn();
    int writen();

    void process_routine();
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

    static inline std::set<http*> httpConns;
    void closeConn(http *);
    bool connState();
    void setFd(int fd);
    int getFd();
    void reset();
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