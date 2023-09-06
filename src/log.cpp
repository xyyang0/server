#include "../headers/log.h"
#include <tuple>
log::log(int open_log):openLog(open_log){
    logfile = dateStr();
}
log::~log(){
    closefile();
}

int log::openfile(){
    filefd = open(logfile.c_str(),O_WRONLY|O_CREAT,
                S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    return filefd;
}


void log::openlog(int open_log){
    mtx.lock();
    openLog = open_log;
    mtx.unlock();
}

void log::closefile(){
    close(filefd);
}

log* log::instance(){
    static log l;
    return &l;
}

std::string log::dateStr(){
    time_t t = time(nullptr);
    struct tm *ptm = localtime(&t);
    
    char buf[256]{0};
    sprintf(buf,"%d-%d-%d_%.2d:%.2d:%.2d",ptm->tm_year+1900,ptm->tm_mon+1,
                ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    return std::string(buf);
}

void log::write_in(std::string &s){
    mtx.lock();
    if(cur_count >= max_count){
        filefd = open(logfile.c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    }else{
        filefd = open(logfile.c_str(),O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    }
    if(filefd == -1){
        perror("open file:");
        kill(getpid(),SIGTERM);
    }
    cur_count++;
    write(filefd,s.c_str(),s.length());
    close(filefd);
    mtx.unlock();
}

bool log::check_args(const std::string &fmt,int cnt){
    int fmt_cnt = 0;
    for(int i = 0; i < fmt.length(); i++){
        char c = fmt[i];
        if(c == '%'){
            if(i+1 == fmt.length() || (fmt[i+1] != 'd' && fmt[i+1] != 's')){
                error_log("invalid format-string:%s",fmt.c_str());
                return false;
            }else{
                fmt_cnt++;
            }
        }
    }
    if(fmt_cnt != cnt){
        error_log("invalid argument count");
        return false;
    }
    return true;
}