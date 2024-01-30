#include "../h/riscv.hpp"

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {

    uint64 volatile a4, a3, a2, a1, a0;
    __asm__ volatile ("mv %[a4], a4" : [a4] "=r" (a4));
    __asm__ volatile ("mv %[a3], a3" : [a3] "=r" (a3));
    __asm__ volatile ("mv %[a2], a2" : [a2] "=r" (a2));
    __asm__ volatile ("mv %[a1], a1" : [a1] "=r" (a1));
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r" (a0));

    uint64 volatile scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        //mem_alloc
        if(a0 == 0x01){
            uint64 a1_n = a1;
            void * res = MemoryAllocator::getInstanceOfMemAlloc().mem_alloc(a1_n);
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //mem_free
        else if(a0 == 0x02){
            void * a1_n = (void * )a1;
            int res = MemoryAllocator::getInstanceOfMemAlloc().mem_free(a1_n);
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //thread_create
        else if(a0 == 0x11){
            void* stack_space = (void*) a4;
            thread_t* handle = (thread_t*) a3;
            void (*start_routine)(void*) = (void (*)(void*)) a2;
            void* arguments = (void*) a1;
            *handle = TCB::createThread(stack_space,start_routine, arguments);
            int res = 0;
            if(*handle == nullptr) res = -1;
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //exit
        else if(a0 == 0x12){
            int res = TCB::exit();
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //dispatch
        else if(a0 == 0x13){
            TCB::dispatch();
        }
        //join
        else if(a0 == 0x14){
            thread_t handle = (thread_t) a1;
            TCB::runningThread->join(handle);
        }
        //sem_open
        else if(a0 == 0x21){
            SemaphoreC** handleSem = (SemaphoreC**)  a1;
            *handleSem = SemaphoreC::openSem(a2);
            int res = 0;
            if(*handleSem == nullptr) res = -1;
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //sem_close
        else if(a0 == 0x22){
            SemaphoreC* handleSem = (SemaphoreC*)  a1;
            int res = handleSem->closeSem();
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //sem_wait
        else if(a0 == 0x23){
            SemaphoreC* handleSem = (SemaphoreC*)  a1;
            int res = handleSem->semWait();
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //sem_signal
        else if(a0 == 0x24){
            SemaphoreC* handleSem = (SemaphoreC*)  a1;
            int res = handleSem->semSignal();
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //getc
        else if(a0 == 0x41){
            char c = __getc();
            __asm__ volatile ("mv a0, %[c]" : : [c]"r" (c));
        }
        //putc
        else if(a0 == 0x42){
            char c = (char)a1;
            __putc(c);
        }
        //switch user
        else if(a0 == 0x45){
            uint64 mask = 1 << 8;
            uint64 mask1 = 1 << 1;
            mc_sip(mask1);
            w_sstatus(sstatus);
            mc_sstatus(mask);
            w_sepc(sepc);
            return;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
//        uint64 volatile sepc = r_sepc() + 4;
//        uint64 volatile sstatus = r_sstatus();
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
       /* TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::runningThread->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        w_sstatus(sstatus);
        w_sepc(sepc);*/
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    /*
    else
    {
        // unexpected trap cause
    }*/
}