#ifndef UTIL_H_
#define UTIL_H_
#include "../headers/Util.hpp"
#include <sys/fcntl.h>
void setnonblock(int fd){
    int flags = fcntl(fd,F_GETFD,0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFD,flags);
}
#endif