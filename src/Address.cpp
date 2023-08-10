#include "../headers/Address.hpp"
#include <string>
#include <cstring>
Address::Address():len(sizeof(addr)){

}
Address::~Address(){

}
Address::Address(const char *ip,const char *port):len(sizeof(addr)){
    bzero(&addr,sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = PF_INET;
    addr.sin_port = htons(std::stoul(port));
}