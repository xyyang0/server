#include "../headers/timerHeap.hpp"
#include <stdexcept>

timerManager* timerManager::getInstance(){
    static timerManager m;
    return &m;
}

timerManager::timerManager(){
    timerArr.push_back(nullptr);
}

timerManager::~timerManager(){
    for(int i = timerArr.size()-1; i >= 1; i--){
        delete timerArr[i];
        timerArr.pop_back();
    }
}

int timerManager::size(){
    return timerArr.size()-1;
}

timer* timerManager::operator[](int idx){
    if(idx > size()){
        throw "out of range";
    }
    return timerArr[idx+1];
}

timer *timerManager::front(){
    if(!empty())
        return timerArr[1];
    throw "visit empty container";
}


timer *timerManager::back(){
    if(!empty())
        return timerArr.back();
    throw "visit empty container";
}

void timerManager::remove(int idx){
    if(idx > size()){
        throw "out of range";
    }
    timer *t = timerArr[idx+1];
    std::swap(timerArr[idx+1],timerArr.back());
    timerArr.pop_back();
    shiftDown(idx+1);
    delete t;
}

void timerManager::delay(timer *t){
    for(int i = 1; i < timerArr.size(); i++){
        if(timerArr[i] == t){
            shiftDown(i);
            return;
        }
    }
    throw "unkonwn timer";
}


void timerManager::buildHeap(){
    if(!empty()){
        for(int i = timerArr.size()/2; i >= 1; i--){
            shiftDown(i);
        }
    }
}

void timerManager::shiftDown(int idx){
    int left = idx*2,right = idx*2+1;
    int minid = idx;
    if(left < timerArr.size() && timerArr[left]->m_expire < timerArr[idx]->m_expire){
        minid = left;
    }
    if(right < timerArr.size() && timerArr[right]->m_expire < timerArr[minid]->m_expire){
        minid = right;
    }
    if(minid != idx){
        std::swap(timerArr[minid],timerArr[idx]);
        shiftDown(minid);
    }
}

void timerManager::shiftUp(int idx){
    while(idx/2 > 0 && timerArr[idx/2] > timerArr[idx]){
        std::swap(timerArr[idx/2],timerArr[idx]);
        idx /= 2;
    }
}

void timerManager::insert(timer *t){
    timerArr.push_back(t);
    shiftUp(timerArr.size()-1);
}

bool timerManager::empty(){
    return timerArr.size() == 1;
}

uint64_t timerManager::time_ms(){
    auto curr_time = std::chrono::system_clock::now();
    auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(curr_time);
    return ms.time_since_epoch().count();
}
