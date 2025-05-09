#include <syscall.h>
#include <stdint.h>
#include <linkedListAllocator.h>
#include <usart.h>
#include <libc.h>


void *syscall0(size_t n){
    return malloc_internal(n);
}

void syscall1(void *p){
    free_internal(p);
}

void syscall2(struct task *t){
    sched_add(t);
}

void syscall3(uint32_t pid){
    sched_remove(pid);
}

void syscall4(uint32_t n){
    asm volatile("cpsid if");
    char tmp[4]; 
    uart_sendstr("Fault at ");
    itoa(n, tmp, 10);
    uart_sendstr(tmp);
    uart_sendstr("\r\n");


    while(1);
}

uint32_t syscall(uint32_t num, uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3){
    register uint32_t _r7 asm("r7") = num;
    register uint32_t _r3 asm("r3") = r3;
    register uint32_t _r2 asm("r2") = r2;
    register uint32_t _r1 asm("r1") = r1;
    register uint32_t _r0 asm("r0") = r0;

    asm volatile("svc 0x0"
            : "=r"(_r0)
            : "r"(_r0), "r"(_r1), "r"(_r2), 
            "r"(_r3), "r"(_r7)
            : "memory");


    return (uint32_t)_r0;
}

void *malloc(size_t n){
    return (void*)syscall(0, n, 0, 0, 0);
}

void free(void *p){
    syscall(1, (uint32_t)p, 0, 0, 0);
}

void exec(struct task *task){
    //syscall(2, (uint32_t)task, 0, 0, 0);

    sched_add(task);
}

void kill(uint32_t pid){
    //syscall(3, pid, 0, 0, 0);
    
    for(int i = 1; i < 10; ++i)
        sched_remove(i);
}


void memfault(uint32_t n){
    syscall(4, n, 0, 0, 0);
}
