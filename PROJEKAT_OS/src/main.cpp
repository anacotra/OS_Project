//
// Created by os on 1/22/24.
//

#include "../h/riscv.hpp"

TCB* tmain;
TCB* usermain;


extern void userMain();

/*
 * //TCB* t1;
//TCB* t2;
//TCB* t3;
//SemaphoreC* sem;
//TCB* idle_t;
struct arg{
    char fa;
    char fb;
};
class Struct1{
public:
    uint64 x;
    uint64 y;
};

int func(){
    int fa = 1;
    __putc('1');
    thread_join(t2);
    int fb = 2;
    __putc('2');
    return fa+fb;
}
int func1(){
    int a = 1;
    __putc('a');
    return a;
}
int func2(){
    int fa = 1;
    __putc('c');
//    thread_dispatch();
    sem_signal(sem);
    int fb = 2;
    __putc('d');
    thread_dispatch();
    return fa+fb;
}
int funcbez(arg* amn){
    char fa = amn->fa;
    __putc(fa);
    thread_dispatch();
    char fb = amn->fb;
    __putc(fb);
    thread_dispatch();
    return fa+fb;
}
//
//void idle(){
//    while(true){
//        thread_dispatch();
//    }
//}
*/

void userMainWrapper(void * arg){
    userMain();
}

int main(){
    //vektor tabela i prebacivanje u vektorski rezim
    Riscv::w_stvec((uint64) &Riscv::vectorTable | 1);
    //omogucava prekid od tajmera
    Riscv::ms_sie(Riscv::SIE_SEIE);

    //omoguci prekid od tajmera
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    //main thread
    thread_create(&tmain, nullptr, nullptr);
    TCB::runningThread = tmain;

    //idle thread
    thread_create(&Scheduler::idle, reinterpret_cast<void (*)(void *)>(Scheduler::idleJob), nullptr);
    Scheduler::put(Scheduler::idle);

    switchUser();

    //user thread
    thread_create(&usermain, &userMainWrapper, nullptr);


    while(!usermain->isFinished()){
        thread_dispatch();
    }

    return 0;
}

/*
    size_t size = sizeof(Struct1) ;
    Struct1* ptr = (Struct1*) mem_alloc(size);
    ptr->x = 0x1;
    ptr->y = 0x2;
    Struct1* ptr1 = (Struct1*) mem_alloc(size);
    ptr1->x = 0x3;
    ptr1->y = 0x4;
    mem_free(ptr);
    //mem_free(ptr1);
    Struct1* ptr2 = (Struct1*) mem_alloc(size);
    ptr2->x = 0x5;
    ptr2->y = 0x6;
    mem_free(ptr1);
    mem_free(ptr2);


//    arg* arguments =  (arg*) mem_alloc(sizeof(arg));
//    arguments->a = 'a';
//    arguments->b = 'b';

//    thread_create(&t1, reinterpret_cast<void (*) (void*)>(func), nullptr);
//    thread_create(&t2, reinterpret_cast<void (*) (void*)>(func1), nullptr);
//    thread_create(&t3, reinterpret_cast<void (*) (void*)>(funcbez), arguments);



//    thread_join(t3);


//    sem_open(&sem, 0);
//    sem_wait(sem);
//    sem_signal(sem);
//    sem_close(sem);


//    while(!t2->isFinished() || !t3->isFinished()){
//        thread_dispatch();
//    }
//    int u = sem_close(sem);
//    if(u < 0){
//        __putc('f');
//    } else if (u == 0){
//        __putc('t');
//    } else
//        __putc('i');

    __putc('i');
    __putc('\n');
int main(){

    //Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    // Kreirajte instancu MemoryAllocator
    MemoryAllocator& allocator = MemoryAllocator::getInstanceOfMemAlloc();

    // Testiranje mem_alloc
    void* allocatedMemory = allocator.mem_alloc(100); // Pravimo rezervaciju od 100 bajta
    if (allocatedMemory == nullptr) {
        __putc('d');
    } else {
        __putc('n');
    }

    // Testiranje mem_free
    int freeResult = allocator.mem_free(allocatedMemory);
    if (freeResult == 0) {
        __putc('n');
    } else {
        __putc('d');
    }
    __putc('\n');

    __putc('O');
    __putc('S');
    __putc('1');
    __putc('\n');
    __putc('\n');

    while (1){
        char character = __getc();
        __putc(character + 30);
    }

    return 0;
}*/