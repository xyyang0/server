#pragma once
#include "Address.hpp"
#include "TcpSocket.hpp"
#include "Epoll.hpp"
#include "threadpool.hpp"
#include "timer.hpp"
#include "Util.hpp"
#include "http.hpp"
#include "objectPool.hpp"
#include "timerHeap.hpp"

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <signal.h>
class webServer{
public:
    webServer(const std::string& ip,const std::string& oprt);
    ~webServer();

    void launch();
private:
    void init();
    void listen();
    void eventLoop();

    void newConnection();
    void insertNewTimer(int fd);
    void removeNotAliveTimer();
    uint64_t expireTime();

    void readWriteEvent(int fd,int type);
    void tick();
    Object<http> *whichObject(int fd);

    static void sigHandler(webServer *w);
    static void sigReceiver(int sig);
    static inline int signum{0};
private:
    TcpSocket* servSock;
    Address* servAddr;
    Epoll* Ep;
    ThreadPool* thPool;
    timerManager *tManager;
    ObjectPool<http> *httpPool;
private:
    std::map<int,Object<http>*> objs;
    bool stop{false};
    int timerSlot{5};
};
