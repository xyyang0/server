#include "timer.hpp"
#include <vector>
#include <map>
#include <chrono>
class timerManager{
public:
    timerManager();
    ~timerManager();
    
    void insert(timer *t);
    bool empty();
    void delay(timer *t);
    void remove(int idx);
    int size();

    static uint64_t time_ms();
    static timerManager *getInstance();
public:
    timer *operator[](int idx);
    timer *front();
    timer *back();
private:
    void buildHeap();
    void shiftDown(int idx);
    void shiftUp(int idx);
    std::vector<timer *> timerArr;
};