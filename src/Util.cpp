#include "../headers/Util.hpp"
#include <sys/fcntl.h>
void setnonblock(int fd){
    int flags = fcntl(fd,F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL,flags);
}

std::vector<std::string> split(const std::string &str,const std::string& token){
   std::string::size_type pos;
   std::vector<std::string> res;
   std::string::size_type size = str.size();
   int i;
   for(i = 0; i < size; ++i){
      pos = str.find(token,i);
      if(pos < size){
         res.push_back( str.substr(i,pos-i) );
         i = pos + token.size() - 1;
      }else{
         break;
      }
   }
   res.push_back( str.substr(i,str.size()-1) );
   return res;
}