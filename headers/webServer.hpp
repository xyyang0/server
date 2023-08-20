#pragma once
#include "Address.hpp"
#include "TcpSocket.hpp"
#include "Epoll.hpp"
#include "threadpool.hpp"
#include "timer.hpp"
#include "Util.hpp"
#include "http.hpp"
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
    void readEvent(int fd);
    void writeEvent(int fd);
    void tick();
    timer *whichTimer(int fd);

    static void sigHandler(webServer *w);
    static void sigReceiver(int sig);
    static inline int signum{0};
private:
    TcpSocket* servSock;
    Address* servAddr;
    Epoll* Ep;
    ThreadPool* thPool;
    timerManager *tManager;
    std::map<int,timer *> timers;
    bool stop{false};
    int timerSlot{5};
};
