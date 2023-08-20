#include "../headers/webServer.hpp"
webServer::webServer(const std::string& ip,const std::string& port):
    servSock(new TcpSocket{}),
    servAddr(new Address{ip,port}),
    Ep(new Epoll),
    thPool(new ThreadPool),
    tManager(timerManager::getInstance())
{

}

webServer::~webServer(){
    delete servSock;
    delete servAddr;
    delete Ep;
    delete thPool;
}

void webServer::launch(){
    init();
    listen();
    alarm(timerSlot);
    eventLoop();
}
void webServer::init(){
    addSig(SIGALRM,webServer::sigReceiver);    
    addSig(SIGINT,webServer::sigReceiver);    
    addSig(SIGTERM,webServer::sigReceiver);    

    Ep->addFd(servSock->getFd(),EPOLLIN);
    
    struct linger opt{1,1};
    setsockopt(servSock->getFd(),SOL_SOCKET,SO_LINGER,&opt,sizeof(opt));
    int reuseopt = 1;
    setsockopt(servSock->getFd(),SOL_SOCKET,SO_REUSEADDR,&reuseopt,sizeof(reuseopt));
}

void webServer::sigReceiver(int sig){
    signum = sig;
}

void webServer::sigHandler(webServer *w){
    if(signum == 0){
        return;
    }else if(signum == SIGINT || signum == SIGTERM){
        w->stop = true;
    }else if(signum == SIGALRM){
        w->tick();
        signum = 0;
        alarm(w->timerSlot);
    }
}
void webServer::listen(){
    servSock->bind(servAddr);
    servSock->listen();
}

void webServer::newConnection(){
    int acceptfd = servSock->accept();
    if(acceptfd > 0){
        setnonblock(acceptfd);
        Ep->addFd(acceptfd, EPOLLIN | EPOLLET | EPOLLONESHOT);

        http *ht = new http{acceptfd};
        uint64_t expire = timerManager::time_ms()+timerSlot*2000;
        timer *t = new timer{expire,ht,Ep};
        t->callback(http::timerCallback,t);
        tManager->insert(t);
        timers[acceptfd] = t;
    }
}

void webServer::readEvent(int fd){
    timer *t = whichTimer(fd);
    if(t != nullptr && t->hptr != nullptr){
        t->m_expire += timerSlot*1000;
        tManager->delay(t);
        thPool->addTask(http::httpProcessRead,t);
    }
}

timer *webServer::whichTimer(int fd){
    auto it = timers.find(fd);
    if(it != timers.end()){
        return it->second;
    }
    return nullptr;
}
void webServer::writeEvent(int fd){
    timer *t = whichTimer(fd);
    if(t != nullptr && t->hptr != nullptr){
        t->m_expire += timerSlot*1000;
        tManager->delay(t);
        thPool->addTask(http::httpProcessWrite,t);
    }
}

void webServer::eventLoop(){
    while(!stop){
        std::vector<epoll_event> pollEvents = Ep->eventLoop();
        int listenfd = servSock->getFd();
        for(const auto &event : pollEvents){
            if(event.data.fd == listenfd){ /*新用户连接*/
                newConnection();
            }else if(event.events & EPOLLIN){ /*读事件*/
                readEvent(event.data.fd);
            }else if(event.events & EPOLLOUT){ /*写事件*/
                writeEvent(event.data.fd);
            }else{
                std::cerr << strerror(errno) << std::endl;
                break;
            }
        }
        sigHandler(this);
    }
}

void webServer::tick(){
    uint64_t ms = timerManager::time_ms();
    while(!tManager->empty()){
       timer *t = tManager->front(); 
       if(t->m_expire > ms){
            break;
       }
        t->func();
        tManager->remove(0);
    }
}