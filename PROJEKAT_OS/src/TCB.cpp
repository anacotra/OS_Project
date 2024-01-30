//
// Created by os on 1/22/24.
//

#include "../h/TCB.hpp"

TCB* TCB::runningThread = nullptr;
Finished* TCB::finishedHead = nullptr;

TCB* TCB::createThread(void* stack_space, Body body, void * arguments){
    TCB* newt = TCBCreate( stack_space, body, arguments);
    if (body != nullptr) { Scheduler::put(newt); }
    return newt;
}

TCB* TCB::createThreadCPP(void* stack_space, Body body, void * arguments){
    TCB* newt = TCBCreate( stack_space, body, arguments);
    return newt;
}
int TCB::startThreadCPP(TCB* handle){
    if (handle->body != nullptr) { Scheduler::put(handle); return 0;}
    return -1;
}

TCB* TCB::TCBCreate(void* stack_space, Body body, void * arguments){
    TCB* newThread = (TCB*)MemoryAllocator::getInstanceOfMemAlloc().mem_alloc((sizeof(TCB) + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE);
    newThread->setBody(body);
    newThread->stack = stack_space;
    newThread->context.ra = (uint64) &threadWrapper;
    newThread->context.sp = newThread->body ? (uint64) newThread->stack + DEFAULT_STACK_SIZE : 0;
    newThread->arguments = arguments;
    newThread->queueHead = nullptr;
    newThread->finished = false;
    newThread->blocked = false;
    return newThread;
}

void TCB::yield(){
    uint64 a0 = 0x13;
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r" (a0));
    __asm__ volatile ("ecall");
}

void TCB::threadWrapper(){
    Riscv::popSppSpie();
    runningThread->body(runningThread->arguments);
    thread_exit();
}

int TCB::exit(){
    if(!TCB::runningThread->isFinished()){
        TCB::runningThread->setFinished(true);
        while(runningThread->queueHead){
            Queue* old = runningThread->queueHead;
            runningThread->queueHead = runningThread->queueHead->next;
            TCB* thr = old->que;
            thr->setQueued(false);
            Scheduler::getInstanceOfScheduler().put(thr);
            MemoryAllocator::getInstanceOfMemAlloc().mem_free(old);
        }
        TCB::insertFinished(TCB::runningThread);
        TCB::dispatch();
        return 0;
    }
    return -1;
}

void TCB::deleteFinThread(){
    if(finishedHead != nullptr){
        Finished* old = finishedHead;
        finishedHead = finishedHead->next;
        MemoryAllocator::getInstanceOfMemAlloc().mem_free(old->fin->stack);
        MemoryAllocator::getInstanceOfMemAlloc().mem_free(old->fin);
        MemoryAllocator::getInstanceOfMemAlloc().mem_free(old);
    }
    return;
}

void TCB::insertFinished(TCB* thr){
    Finished* n = (Finished*) MemoryAllocator::getInstanceOfMemAlloc().mem_alloc((sizeof(Finished) + MEM_BLOCK_SIZE - 1)/ MEM_BLOCK_SIZE);
    if(finishedHead){
        n->next = finishedHead;
    }
    finishedHead = n;
}

void TCB::join(TCB* handle){
    if(!handle->isFinished()){
        handle->setQueued(true);
        Queue* q = (Queue*) MemoryAllocator::getInstanceOfMemAlloc().mem_alloc(sizeof(Queue));
        q->que = this;
        q->next = nullptr;
        if(handle->queueHead) q->next = handle->queueHead;
        handle->queueHead = q;
        TCB * oldThread = runningThread;
        runningThread = Scheduler::get();

        TCB::contextSwitch(&oldThread->context, &runningThread->context);
    }
    return;
}

void TCB::dispatch(){
    TCB * oldThread = runningThread;
    if(!oldThread->isFinished() && !oldThread->isBlocked()) { Scheduler::put(oldThread); }
    runningThread = Scheduler::get();

    TCB::contextSwitch(&oldThread->context, &runningThread->context);
}

