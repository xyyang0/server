#pragma once
#include <iostream>
#include <vector>

template <typename Ty>
struct Object{
    Object():object(new Ty{}){}
    ~Object(){ delete object; }
    Ty *object;
    bool active{false};
};

template <typename Ty>
class ObjectPool{
public:
    ObjectPool(int num = Unit);
    ~ObjectPool();

    Object<Ty>* seekFree();
    std::string what();
private:
    void expand();
    enum { Unit = 1024 };
    std::vector<Object<Ty> *> Objects;
    std::string err;
};


template <typename Ty>
ObjectPool<Ty>::ObjectPool(int num){
    if(num == 0){
        throw "count of object can not be zero";
    }
    for(int i = 0; i < num; i++){
        Objects.emplace_back(new Object<Ty>{});
    }
}

template <typename Ty>
ObjectPool<Ty>::~ObjectPool(){
    for(auto ptr : Objects){
        delete ptr;
    }
    Objects.clear();
}

template <typename Ty>
Object<Ty>* ObjectPool<Ty>::seekFree(){
    for(auto ptr : Objects){
        if(ptr->active == false){
            ptr->active = true;
            err = "success";
            return ptr;
        }
    }
    int size = Objects.size();
    if(size == Unit*4){
        err = "too many objects";
        return nullptr;
    }
    expand();
    Objects[size]->active = true;
    return Objects[size];
}

template <typename Ty>
void ObjectPool<Ty>::expand(){
    int size = Objects.size();
    for(int i = 0; i < size*2; i++){
        Objects.emplace_back(new Object<Ty>{});
    }
}