#include "../h/MemoryAllocator.hpp"


MemoryAllocator::MemoryAllocator() {
    if(head == nullptr){
        head = (FreeBlock*) HEAP_START_ADDR;
        head->next = nullptr;
        head->prev = nullptr;
        head->size = (uint64) ((uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR - sizeof (FreeBlock));
    }
}

void * MemoryAllocator:: mem_alloc(size_t size) {
    if(size <= 0) return nullptr;

    size_t newsize = size * MEM_BLOCK_SIZE + sizeof(FreeBlock);

    for (FreeBlock* cur = head; cur != nullptr; cur=cur->next) {
        if (cur->size < newsize) continue;

        if (cur->size - newsize < sizeof(FreeBlock)) {
            // Nema preostalog dela
            if (cur->prev) cur->prev->next = cur->next;
            else head = cur->next;

            if (cur->next) cur->next->prev = cur->prev;

        } else {
            //Preostaje neki deo
            FreeBlock *newfrgm = (FreeBlock *) ((uint64) cur + newsize);
            if (cur->prev) cur->prev->next = newfrgm;
            else head = newfrgm;

            if (cur->next) cur->next->prev = newfrgm;

            newfrgm->prev = cur->prev;
            newfrgm->next = cur->next;
            newfrgm->size = cur->size - newsize;
        }
        cur->size = newsize;
        return (char *) cur + sizeof(FreeBlock);
    }
    return nullptr;
}

int MemoryAllocator::tryToJoin(FreeBlock* cur) {
    if (!cur) return 0;
    if (cur->next && (uint64)cur + cur->size == (uint64)(cur->next)) {
        // Uklanja cur->next segment:
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if (cur->next) cur->next->prev = cur;
        return 1;
    } else
        return 0;
}

MemoryAllocator& MemoryAllocator::getInstanceOfMemAlloc() {
    // Vraća instancu MemoryAllocator
    static MemoryAllocator instance;
    return instance;
}

int MemoryAllocator::mem_free(void *addr) {
    if(addr == nullptr || (uint64)addr > (uint64)HEAP_END_ADDR
    || (uint64)addr < (uint64)HEAP_START_ADDR) return  -1;
    // Trazimo gde da umetnemo:
    FreeBlock* cur = nullptr;
    if (!head || (uint64)addr < (uint64)head)
        cur = nullptr; // ubacimo kao prvi
    else
        for (cur = head; cur->next != nullptr && (uint64)addr > (uint64)(cur->next);
             cur = cur->next);
    // Ubacimo novi posle cur:
    FreeBlock* newBlk = (FreeBlock*)((uint64)addr - sizeof (FreeBlock));
    newBlk->prev = cur;
    if (cur) newBlk->next = cur->next;
    else newBlk->next = head;

    if (newBlk->next) newBlk->next->prev = newBlk;

    if (cur) cur->next = newBlk;
    else head = newBlk;

    // Spajamo sa prethodnim i trenutnim:
    tryToJoin(newBlk);
    tryToJoin(cur);
    return 0;
}
