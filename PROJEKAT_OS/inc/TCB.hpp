//
// Created by os on 1/22/24.
//Deo koda preuzet sa casova vezbi.

#ifndef PROJECT_OS_TCB_HPP
#define PROJECT_OS_TCB_HPP

#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "MemoryAllocator.hpp"
#include "riscv.hpp"


struct Finished{
    TCB* fin;
    Finished* next;
};
struct Queue{
    TCB* que;
    Queue* next;
};

class TCB {
public:
    //uint64 getTimeSlice() const { return timeSlice; }

    //opsti tip za fije
    using Body = void (*)(void *);

    //napravi nit
    static TCB* createThread(void* stack_space, TCB::Body body, void * arguments);
    static TCB* createThreadCPP(void* stack_space, Body body, void * arguments);
    static int startThreadCPP(TCB* handle);

    //context switch
    static void yield();

    //fija koja se poziva promenu konteksta
    static void dispatch();

    //nit se vise ne koristi
    static int exit();

    //nit koja se izvrsava trenutno
    static TCB* runningThread;

    //za red zavrsenih niti
    static Finished* finishedHead;
    static void deleteFinThread();
    static void insertFinished(TCB* thr);

    //za red niti koje se cekaju da se zavrse kako bismo se mi zavrsili
    void join(TCB* handle);

    //dealociraj stek
    ~TCB() { MemoryAllocator::getInstanceOfMemAlloc().mem_free(stack); }

    //da li je zavrsena nit (postavljen finished flag) i postavi vrednost za finished
    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }

    //da li je blokirana nit na nekom semaforu
    bool isBlocked() const { return blocked; }
    void setBlocked(bool value) { blocked = value; }

    //da li ceka na neku drugu nit
    bool isQueued() const { return queued; }
    void setQueued(bool value) { queued = value; }

    //postavlja body
    void setBody(Body b) { body = b; }

private:
    //konstruktor
    static TCB* TCBCreate(void* stack_space, Body body, void * arguments);

    /*timeSlice(timeSlice),*/

    //struktura u kojoj cuvamo registar ra i stek pointer
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    //element tipa Body
    Body body;
    //pokazivac na stek
    void *stack;
    //element u kome cuvamo ra i sp ove niti
    Context context;
    //argumenti fije koja se zove u niti
    void * arguments;
    //red niti koje ova nit ceka da se zavrse
    Queue* queueHead = nullptr;
    //uint64 timeSlice;
    //promenljive koje govore da li je zavrsena, zatvorena ili blokirana nit
    bool finished;
    bool blocked;
    bool queued;

    //prijateljska klasa kako bismo mogli da koristimo privatne metode
    friend class Riscv;
    //fija koja se poziva kada nastaje nova nit
    static void threadWrapper();
    //fija koja se poziva pri promeni konteksta
    static void contextSwitch(Context *oldContext, Context *runningContext);
    //static uint64 timeSliceCounter;
};

#endif //PROJECT_OS_TCB_HPP
