#pragma once

#include <string>
class Buffer{
private:
    std::string buffer;
public:
    Buffer()=default;
    ~Buffer()=default;

    int size();
    const char *getbuf();
    void append(const char *s,int len);
};