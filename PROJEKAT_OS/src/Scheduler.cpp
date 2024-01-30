//
// Created by os on 1/22/24.
//

#include "../h/Scheduler.hpp"

TCB* Scheduler::idle = nullptr;
Sch * Scheduler::tail = nullptr;
Sch * Scheduler::head = nullptr;


void Scheduler::idleJob(void *arg) {
    TCB::deleteFinThread();
    switchUser();
    while(true){
        thread_dispatch();
    }
}

Scheduler& Scheduler::getInstanceOfScheduler(){
    static Scheduler instance;
    return instance;
}

TCB * Scheduler::get(){
    if(!head) return nullptr;

    Sch* elem = head;

    head = head->next;
    if(!head) tail = nullptr;

    TCB* thread = elem->thread;
    MemoryAllocator::getInstanceOfMemAlloc().mem_free(elem);
    return thread;
}

void Scheduler::put(TCB *tcb){
    if(!tcb) return;

    Sch* elem = (Sch*) MemoryAllocator::getInstanceOfMemAlloc().mem_alloc((sizeof(Sch) + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE);
    elem->thread = tcb;
    elem->next = nullptr;

    if(!head) head = elem;
    else tail->next = elem;

    tail = elem;
}