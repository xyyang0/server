#include "../headers/webServer.hpp"
webServer::webServer(const std::string& ip,const std::string& port):
    servSock(new TcpSocket{}),
    servAddr(new Address{ip,port}),
    thPool(new ThreadPool),
    httpPool(new ObjectPool<http>{})
{
    Ep = Epoll::getInstance();
    tManager = timerManager::getInstance();
}

webServer::~webServer(){
    delete servSock;
    delete servAddr;
    delete thPool;
    delete httpPool;
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
    addSig(SIGUSR1,webServer::sigReceiver);    

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

void timerCallback(timer *t){
    t->h->httpClose();
}

uint64_t webServer::expireTime(){
    return timerManager::time_ms()+timerSlot*2000;/*当前时间10s后到期*/
}
void webServer::insertNewTimer(int fd){
    auto ptr = httpPool->seekFree();
    if(ptr != nullptr){
        http *h = ptr->object;
        timer *t = new timer{expireTime(),h};
        t->callback(timerCallback, t);
        h->setHttp(t,fd);
        tManager->insert(t);
        objs[fd] = ptr;
    }
}
void webServer::newConnection(){
    int acceptfd = servSock->accept();
    if(acceptfd > 0){
        setnonblock(acceptfd);
        Ep->addFd(acceptfd, EPOLLIN | EPOLLET | EPOLLONESHOT);
        insertNewTimer(acceptfd);
    }
}

Object<http> *webServer::whichObject(int fd){
    auto it = objs.find(fd);
    if(it != objs.end()){
        return it->second;
    }
    return nullptr;
}

void webServer::readWriteEvent(int fd,int type){
    Object<http> *obj = whichObject(fd);
    if(obj != nullptr){
        http *h = obj->object;
        h->t->m_expire += timerSlot*2000;
        tManager->delay(h->t);
        if(type == 0){          /*读事件*/
            thPool->addTask(http::httpProcessRead,obj);
        }else{                  /*写事件*/
            thPool->addTask(http::httpProcessWrite,obj);
        }
    }
}
void webServer::eventLoop(){
    int listenfd = servSock->getFd();
    while(!stop){
        std::vector<epoll_event> pollEvents = Ep->poll();
        for(const auto &event : pollEvents){
            if(event.data.fd == listenfd){ /*新用户连接*/
                newConnection();
            }else if(event.events & EPOLLIN){ /*读事件*/
                readWriteEvent(event.data.fd,0);
            }else if(event.events & EPOLLOUT){ /*写事件*/
                readWriteEvent(event.data.fd,1);
            }else{
                std::cerr << strerror(errno) << std::endl;
                break;
            }
        }
        sigHandler(this);
        tManager->removeNotAliveTimer();
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