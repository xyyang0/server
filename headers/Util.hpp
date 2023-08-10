#pragma once
#include <sys/fcntl.h>
#include <string>
#include <vector>
void setnonblock(int fd);
std::vector<std::string> split(const std::string &str,const std::string &token);