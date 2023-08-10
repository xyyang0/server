#pragma once

#include <string>
#include <vector>
class Buffer{
private:
    std::string buffer;
public:
    Buffer()=default;
    ~Buffer()=default;

    int size();
    const char *getbuf();
    void append(const char *s,int len);
    void clear();
};