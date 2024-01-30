//
// Created by os on 1/24/24.
//

#include "../h/Semaphore.hpp"


SemaphoreC* SemaphoreC::openSem(int val){
    SemaphoreC  * sem = (SemaphoreC *)MemoryAllocator::getInstanceOfMemAlloc().mem_alloc((sizeof(SemaphoreC ) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    sem->head = nullptr;
    sem->tail = nullptr;
    sem->val = val;
    sem->closed = false;
    return sem;
}

int SemaphoreC::closeSem(){
    while(head){
        BlockedQueue* blk = head;
        TCB* thr = blk->threadBlk;
        thr->setBlocked(false);
        Scheduler::put(thr);
        MemoryAllocator::getInstanceOfMemAlloc().mem_free(blk);
        head = head->next;
    }
    if(head) return -1;
    this->closed = true;
    return 0;
}

int SemaphoreC::semWait(){
    if(closed){
        return -1;
    }
    TCB* thr = TCB::runningThread;
    int v = this->getValue() - 1;
    this->setValue(v);
    if(v < 0){
        thr->setBlocked(true);
        BlockedQueue* newBlk = (BlockedQueue*)MemoryAllocator::getInstanceOfMemAlloc().mem_alloc((sizeof(BlockedQueue)+MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE);
        newBlk->threadBlk = thr;
        newBlk->next = nullptr;

        if(!head) head = newBlk;
        else tail->next = newBlk;
        tail = newBlk;
        TCB::dispatch();
    }
    return 0;
}

int SemaphoreC::semSignal(){
    int v = this->getValue() + 1;
    if(head){
        BlockedQueue* first = head;
        TCB* thr = first->threadBlk;
        thr->setBlocked(false);
        head = head->next;
        if(!head) tail = nullptr;
        MemoryAllocator::getInstanceOfMemAlloc().mem_free(first);
        if(!thr->isBlocked()) Scheduler::put(thr);
    }
    this->setValue(v);
    return 0;
}