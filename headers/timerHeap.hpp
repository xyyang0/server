#include "timer.hpp"

#include <vector>
#include <map>
#include <chrono>
#include <mutex>
class timerManager{
public:
    void insert(timer *t);
    bool empty();
    void delay(timer *t);
    void remove(int idx);
    void remove(timer *t);
    void removeNotAliveTimer();
    int size();

    static uint64_t time_ms();
    static timerManager *getInstance();
public:
    timer *operator[](int idx);
    timer *front();
    timer *back();

    static void lock();
    static void unlock();
private:
    timerManager();
    ~timerManager();

    void buildHeap();
    void shiftDown(int idx);
    void shiftUp(int idx);
    std::vector<timer *> timerArr;
    static inline std::mutex mtx;
};