//
// Created by os on 1/24/24.
//

#ifndef PROJECT_OS_SEMAPHORE_HPP
#define PROJECT_OS_SEMAPHORE_HPP

#include "TCB.hpp"
#include "Scheduler.hpp"

class TCB;

struct BlockedQueue{
    TCB* threadBlk;
    BlockedQueue* next;
};

class SemaphoreC {
public:
    static SemaphoreC* openSem(int val);

    int closeSem();

    int semWait();

    int semSignal();

    int getValue() { return val; }

    void setValue(int va) { val = va; }

private:
    int val;
    bool closed = false;
    BlockedQueue* head = nullptr;
    BlockedQueue* tail = nullptr;
};


#endif //PROJECT_OS_SEMAPHORE_HPP
