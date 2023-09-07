#include "../headers/Address.hpp"
#include <string>
#include <cstring>
Address::Address():len(sizeof(addr)){

}
Address::~Address(){

}

Address::Address(const sockaddr_in & addr_st,socklen_t l):addr(addr_st),len(l){

}
Address::Address(const std::string& ip,const std::string& port):len(sizeof(addr)){
    bzero(&addr,sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_family = PF_INET;
    addr.sin_port = htons(std::stoul(port));
}

std::string Address::get_ip()const{
    return std::string(inet_ntoa(addr.sin_addr));
}

std::string Address::get_port()const{
    return std::to_string(addr.sin_port);
}

struct sockaddr_in Address::get_trivial_addr()const{
    return addr;
}

socklen_t Address::get_addr_len()const{
    return len;
}