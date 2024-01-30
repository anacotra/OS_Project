//
// Created by os on 12/25/23.
//

#ifndef PROJECT_OS_SYSCALL_C_HPP
#define PROJECT_OS_SYSCALL_C_HPP
#include "../lib/hw.h"
#include "../lib/console.h"
#include "MemoryAllocator.hpp"

//Memory
void* mem_alloc (size_t size);
int mem_free (void* addr);

//Thread
class TCB;
typedef TCB* thread_t;
int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit ();
void thread_dispatch ();
void thread_join (thread_t handle);

//SemaphoreM
class SemaphoreC;
typedef SemaphoreC* sem_t;
int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

//Timer
typedef unsigned long time_t;
int time_sleep (time_t t);

//Console
const int EOF = -1;
char getc ();
void putc (char c);

//switch user
void switchUser();

#endif //PROJECT_OS_SYSCALL_C_HPP
