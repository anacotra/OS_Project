//
// Created by os on 12/25/23.
//Deo koda preuzet sa casova vezbi.

#ifndef PROJECT_OS_MEMORYALLOCATOR_HPP
#define PROJECT_OS_MEMORYALLOCATOR_HPP
#include "../lib/hw.h"

struct FreeBlock {
    FreeBlock* prev;
    FreeBlock* next;
    size_t size;
};

class MemoryAllocator {

    FreeBlock *head=nullptr;
    //spajanje sa blokom ispod
    int tryToJoin(FreeBlock* cur);

public:
    MemoryAllocator();

    static MemoryAllocator& getInstanceOfMemAlloc();
    //alokacija memorije
    void *mem_alloc(size_t sz);
    //oslobadjanje memorije
    int mem_free(void* addr);

};

#endif //PROJECT_OS_MEMORYALLOCATOR_HPP
