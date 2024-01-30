//
// Created by os on 1/24/24.
//

#include "../h/syscall_cpp.hpp"

//new & delete
void* operator new (size_t size){
    return mem_alloc(size);
}
void* operator new[] (size_t size){
    return mem_alloc(size);
}
void operator delete (void* obj){
    mem_free(obj);
}
void operator delete[] (void* obj){
    mem_free(obj);
}

//thread
Thread::Thread (void (*body)(void*), void* arg){
    this->body = body;
    this->arg = arg;
}

Thread::~Thread(){
    if(myHandle){
        delete myHandle;
    }
}

int Thread::start(){
    int b = thread_create(&myHandle, &tStart, this);
    return b;
}

void Thread::join(){
    thread_join(myHandle);
}

void Thread::dispatch (){
    thread_dispatch();
}

int Thread::sleep (time_t t){
    return 0;
}

Thread::Thread (){
    this->body = nullptr;
    this->arg = nullptr;
}

void Thread::tStart(void* arg){
    Thread* t = (Thread*) arg;
    if(t->body){
        t->body(t->arg);
    } else {
        t->run();
    }
};

//semaphore
Semaphore::Semaphore(unsigned init){
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore (){
    sem_close(myHandle);
}

int Semaphore::wait (){
    int b = sem_wait(myHandle);
    return b;
}
int Semaphore::signal(){
    int b = sem_signal(myHandle);
    return b;
}

char Console::getc (){
    return ::getc();
}
void Console::putc (char c){
    ::putc(c);
}