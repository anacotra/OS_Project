//
// Created by os on 12/25/23.
//

#include "../h/syscall_c.hpp"

//Memory
void* mem_alloc (size_t size){
    uint64 oper = 0x01;
    size_t newSize = (size+ MEM_BLOCK_SIZE-1) / MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %[newSize]" : : [newSize]"r" (newSize));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return (void *) res;
}
int mem_free (void* addr){
    void * address = addr;
    uint64 oper = 0x02;
    __asm__ volatile ("mv a1, %[address]" : : [address]"r" (address));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

//Thread
int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    void * stack_space = nullptr;
    thread_t* handleThread = handle;
    void (*sr) (void*) = start_routine;
    void* arguments = arg;
    uint64 oper = 0x11;
    if (start_routine) {
        stack_space = mem_alloc(DEFAULT_STACK_SIZE);
        if (stack_space == nullptr) {
            return -1;
        }
    }
    __asm__ volatile ("mv a4, %[stack_space]" : : [stack_space] "r" (stack_space));
    __asm__ volatile ("mv a3, %[handleThread]" : : [handleThread] "r" (handleThread));
    __asm__ volatile ("mv a2, %[sr]" : : [sr] "r" (sr));
    __asm__ volatile ("mv a1, %[arguments]" : : [arguments] "r" (arguments));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    int res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}
int thread_exit (){
    uint64 oper = 0x12;
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    int res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}
void thread_dispatch (){
    uint64 oper = 0x13;
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
}
void thread_join (thread_t handle){
    thread_t handleThread = handle;
    uint64 oper = 0x14;
    __asm__ volatile ("mv a1, %[handleThread]" : : [handleThread] "r" (handleThread));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
}
//SemaphoreC
int sem_open (sem_t* handle, unsigned init){
    if(!handle) return -1;
    sem_t * handleSem = handle;
    unsigned i = init;
    uint64 oper = 0x21;
    __asm__ volatile ("mv a2, %[i]" : : [i] "r" (i));
    __asm__ volatile ("mv a1, %[handleSem]" : : [handleSem] "r" (handleSem));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    int res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}
int sem_close (sem_t handle){
    if(!handle) return -1;
    int res = -1;
    sem_t handleSem = handle;
    uint64 oper = 0x22;
    __asm__ volatile ("mv a1, %[handleSem]" : : [handleSem] "r" (handleSem));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}
int sem_wait (sem_t id){
    if(!id) return -1;
    sem_t handleSem = id;
    uint64 oper = 0x23;
    __asm__ volatile ("mv a1, %[handleSem]" : : [handleSem] "r" (handleSem));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    int res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}
int sem_signal (sem_t id){
    if(!id) return -1;
    sem_t handleSem = id;
    uint64 oper = 0x24;
    __asm__ volatile ("mv a1, %[handleSem]" : : [handleSem] "r" (handleSem));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    int res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int time_sleep (time_t t){
    return 0;
}

char getc (){
    uint64 oper = 0x41;
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    char res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}
void putc (char ch){
    char c = ch;
    uint64 oper = 0x42;
    __asm__ volatile ("mv a1, %[c]" : : [c] "r" (c));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
}

//switch user
void switchUser(){
    uint64 oper = 0x45;
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
}