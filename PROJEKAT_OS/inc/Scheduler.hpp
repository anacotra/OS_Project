//
// Created by os on 1/22/24.
//Deo koda preuzet sa casova vezbi.

#ifndef PROJECT_OS_SCHEDULER_H
#define PROJECT_OS_SCHEDULER_H

#include "MemoryAllocator.hpp"
#include "TCB.hpp"

class TCB;

//struktura koja cuba niti koje cekaju
struct Sch{
    TCB* thread;
    Sch* next;
};

class Scheduler{
private:
    static Sch * tail;
    static Sch * head;
public:

    static Scheduler& getInstanceOfScheduler();
    //idle nit i njena funkcija
    static TCB* idle;
    static void idleJob(void * arg);

    //ukloni prvi
    static TCB *get();

    //dodaj na kraj
    static void put(TCB *tcb);
};


#endif //PROJECT_OS_SCHEDULER_H
