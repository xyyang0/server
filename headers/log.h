#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <iostream>
#include <string>
#include <mutex>

class log{
public:
    enum Level{DEBUG = 0,WARNING,ERROR,FATAL};

    static log* instance();

    void setLevel(Level l){ level = l; }
    int getLevel(){ return level; }
    void openlog(int open_log);

    template <typename ...Args>
    void logcall(int level,const std::string& fmt,Args && ...args);
private:
    log(int open_log = 0);
    ~log();

    int openfile();
    void closefile();


    void write_in(std::string &s);
    std::string dateStr();
    bool check_args(const std::string& fmt,int cnt);

    template <typename ...Args>
    void write_(const std::string& levelStr,const std::string& fmt,Args&& ...args);

public:
    template <typename... Args>
    std::string logMsg(const std::string& levelStr,const std::string& fmt,Args && ...args);

    template <typename... Args>
    void debug_log(const std::string& fmt,Args &&...args);

    template <typename... Args>
    void warning_log(const std::string& fmt,Args &&...args);
    
    template <typename... Args>
    void error_log(const std::string& fmt,Args &&...args);

    template <typename... Args>
    void fatal_log(const std::string& fmt,Args &&...args);
    
private:
    int filefd{-1};
    int cur_count{0};
    int max_count{30};
    std::string logfile;
    std::mutex mtx;
    int level{DEBUG};
    int openLog{0};
};


template <typename ...Args>
std::string log::logMsg(const std::string& levelStr,const std::string& fmt,Args && ...args){
    char buf[1024]{0};
    int size = snprintf(buf,1024,"%s [",levelStr.c_str());
    size += snprintf(buf+size,1024-size,"%s] ",dateStr().c_str());
    size += snprintf(buf+size,1024-size,fmt.c_str(),std::forward<Args>(args)...);

    std::string msg(buf);
    msg += "\n";
    std::cout << buf <<std::endl;
    return msg;
}

template <typename... Args>
void log::debug_log(const std::string& fmt,Args &&...args){
    if(level == DEBUG){
        write_("[debug]",fmt,std::forward<Args>(args)...);
    }
}

template <typename ...Args>
void log::warning_log(const std::string& fmt,Args &&...args){
    if(level <= WARNING){
        write_("[warning]",fmt,std::forward<Args>(args)...);
    }
}

template <typename ...Args>
void log::error_log(const std::string& fmt,Args &&...args){
    if(level <= ERROR){
        write_("[error]",fmt,std::forward<Args>(args)...);
    }
}

template <typename ...Args>
void log::fatal_log(const std::string& fmt,Args &&...args){
   write_("[fatal]",fmt,std::forward<Args>(args)...);
   kill(getpid(),SIGTERM);
}

template <typename ...Args>
void log::write_(const std::string& levelStr,const std::string& fmt,Args&& ...args){
    if(check_args(fmt,sizeof...(args))){
        std::string msg = logMsg(levelStr,fmt,std::forward<Args>(args)...);
        write_in(msg);
    }
}
