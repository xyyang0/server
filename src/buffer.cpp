#include "../headers/buffer.hpp"

int Buffer::size(){
    return buffer.size();
 }

 void Buffer::append(const char *s, int len){
    for(int i = 0; i < len; i++){
        buffer.push_back(s[i]);
    }
 }

 const char *Buffer::getbuf(){
    return buffer.c_str();
 }