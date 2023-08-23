#include "../headers/http.hpp"
#include <thread>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
#define BUFSIZE 1024
void http::setHttp(timer *tp, Epoll *p,int fd){
    clientfd = fd;
    t = tp;
    Ep = p;
}

void http::httpClose(){
    close(clientfd);
    t->h = nullptr;
    t->keep_alive = false;
    t = nullptr;
    Ep = nullptr;
    reset();
}
int http::readn(){
   while(1){
        char buf[BUFSIZE]{'\0'};
        int ret = read(clientfd, buf, BUFSIZE);
        if(ret < 0 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
            return readBuf.size();
        }else if(ret < 0 && errno == EINTR){
            continue;
        }else if(ret == 0){
            return readBuf.size();
        }else{
            readBuf.append(buf,ret);
        }
   } 
}


void http::httpProcessRead(Object<http>* ptr){
    http *h = ptr->object;
    try{
        if(h->readn() == 0){
            h->httpClose();
            ptr->active = false;
            return;
        }
        h->process_request();
        h->Ep->modfd(h->getFd(),EPOLLET | EPOLLOUT | EPOLLONESHOT);
    }catch(const char *err){
        std::cerr << err << std::endl;
        h->reset();
    }
}
void http::httpProcessWrite(Object<http> *ptr){
    http *h = ptr->object;
    h->process_response();
    if(!h->keep_alive){
        h->httpClose();
        ptr->active = false;
        return;
    }
    h->reset();
    h->Ep->modfd(h->getFd(),EPOLLET | EPOLLIN | EPOLLONESHOT);
}


void http::process_request(){
    std::vector<std::string> inputStrs = split(readBuf.getbuf(),"\r\n"); 
    parse_request_line(inputStrs);
    parse_request_header(inputStrs);
    if(method == http_method::POST){
        parse_content(inputStrs);
    }
}

void http::parse_request_line(const std::vector<std::string> &inputStrs){
    if(inputStrs.size() < 1){
        throw "invalid request message";
    }
    std::string header = inputStrs[current_line++];
    /* http header: GET http://localhost:80/index.html http/1.1 */
    std::vector<std::string> headerTokens = split(header," "); 
    if(headerTokens.size() < 3){
        throw "invalid request header:" + header;
    }

    if(headerTokens[0] == "GET"){
        method = http_method::GET;
    }else if(headerTokens[0] == "POST"){
        method = http_method::POST;
    }else{
        throw "unknow request method:" + headerTokens[0];
    }

    url = headerTokens[1];
    version = headerTokens[2];
}
void http::parse_request_header(const std::vector<std::string> &inputStrs){
    for(int i = current_line; i < inputStrs.size(); i++){
        const std::string &s = inputStrs[i];
        if(s.length() == 0){
            current_line = i+1;
            break;
        }
        else if(s.find("Connection:") != std::string::npos){
            if(s.find("keep-alive") != std::string::npos){
                keep_alive = true;
            }else{
                keep_alive = false;
            }
        }else if(s.find("Content-length:") != std::string::npos){
            int pos = strlen("Content-length:");
            while(s[pos] == ' ' || s[pos] == '\t') pos++;
            content_length = std::stoi(s.substr(pos));
        }
    }
}
void http::parse_content(const std::vector<std::string> &inputStrs){
}

int http::check_file(){
    std::string filename = "./res/";
    std::string::size_type pos = url.rfind('/');
    char c = url[pos+1];
    if(c == 0){
        filename += "welcome.html";
    }else if(c == '1'){ 
        filename += "jpg.html";
    }else if(c == '2'){
        filename += "mp4.html";
    }else{
        filename += url.substr(pos+1);
    }

    state = http_state::OK;
    struct stat st;
    if( stat(filename.c_str(),&st) < 0 ){
        filename = "./res/notFound.html";
        state = http_state::BAD_REQUEST;
    }
    stat(filename.c_str(),&st);
    content_length = st.st_size;
    int fd = open(filename.c_str(),O_RDONLY);
    if(fd < 0){
        perror("open:");
        return -1;
    }
    mapaddr = mmap(0,st.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    if(mapaddr == MAP_FAILED){
        perror("mmap");
        return -1;
    }
    close(fd);
    return 0;
}


void http::add_response_line(const char *str){
    char buf[1024];
    int cnt = sprintf(buf,"HTTP/1.1 %s\r\n",str);
    writeBuf.append(buf,cnt);
}

void http::add_response_header(){
    char buf[1024];
    int cnt = 0;
    cnt += sprintf(buf+cnt,"Content-Length:%d\r\n",content_length);
    cnt += sprintf(buf+cnt,"Connection:%s\r\n",keep_alive ? "keep-alive" : "close");
    cnt += sprintf(buf+cnt,"Content-Type:text/html\r\n\r\n");
    writeBuf.append(buf,cnt);
}


void http::add_response(){
    const char *msg = state == http_state::BAD_REQUEST ? "404 Not Found" : "200 OK";
    add_response_line(msg);
    add_response_header();
}

void http::process_write(){
   struct iovec iov[2]; 
   char *s = const_cast<char*>(writeBuf.getbuf());
   iov[0].iov_base = s;
   iov[0].iov_len = writeBuf.size();

   iov[1].iov_base = mapaddr;
   iov[1].iov_len = content_length;

   int iocnt = 2;
   unsigned int totalBytes = writeBuf.size() + content_length,haveWrite = 0;
   unsigned int cnt1 = writeBuf.size(),cnt2 = content_length;
   while(1){
        int nwrite = writev(clientfd,iov,iocnt);
        if(nwrite > 0){
            haveWrite += nwrite;
        }
        else{
            if(errno == EAGAIN){
                if(haveWrite < cnt1){
                    iov[0].iov_base = (s + haveWrite);
                    iov[0].iov_len = cnt1 - haveWrite;
                }else if(haveWrite < totalBytes){
                    iov[0].iov_base = nullptr;
                    iov[0].iov_len = 0;
                    iov[1].iov_base = ((char *)mapaddr + haveWrite - cnt1);
                    iov[1].iov_len = cnt2 - (haveWrite - cnt1);
                }
                continue;
            }
            reset();
            return;
        }
        if(haveWrite >= totalBytes){
            break;
        }
   }
}

void http::process_response(){
    if(check_file() < 0){
        return;
    }
    add_response();
    process_write();
}

int http::getFd(){
    return clientfd;
}
void http::reset(){
    if(mapaddr){
        munmap(mapaddr,content_length);
        mapaddr = nullptr;
    }
    content_length = 0;
    current_line = 0;

    readBuf.clear();
    writeBuf.clear();
}