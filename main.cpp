#include <iostream>
#include "./headers/webServer.hpp"

void help(){
    std::cerr << "Usage: ./build/server [-l [0|1]]" << std::endl << "choose open or close log";
}
int parse_options(int argc,char **argv){
    int open_log = 0;
    if(argc == 2){
        help();
        return -1;
    }
    if(argc == 3){
        if(strcmp(argv[1],"-l") == 0){
            open_log = atoi(argv[2]);
            if(open_log != 1 && open_log != 0){
                help();
                return -1;
            }
        }else{
            std::cerr << "invalid option '" << argv[1] << "'";
            exit(0);
        }
    }
    return open_log;
}
int main(int argc,char **argv){
    int open_log = parse_options(argc,argv);
    log::instance()->openlog(open_log);
    std::string ip = "127.0.0.1",port = "12345";
    webServer Server(ip,port);
    Server.launch();
}
