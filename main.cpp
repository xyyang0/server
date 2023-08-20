#include <iostream>
#include "./headers/webServer.hpp"
int main(int argc,char **argv){
    if(argc < 3){
        std::cerr << "./server [ip] [port]";
        exit(-1);
    }
    webServer Server(argv[1],argv[2]);
    Server.launch();
}
