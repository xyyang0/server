#include "../headers/Address.hpp"
#include <string>
#include <cstring>
Address::Address():len(sizeof(addr)){

}
Address::~Address(){

}
Address::Address(const std::string& ip,const std::string& port):len(sizeof(addr)){
    bzero(&addr,sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_family = PF_INET;
    addr.sin_port = htons(std::stoul(port));
}