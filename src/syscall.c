#include <syscall.h>
#include <stdint.h>

static inline void __disable_irq(){
    asm volatile("cpsid if");
}
static inline void __enable_irq(){
    asm volatile("cpsie if");
}


long syscall0(int n){
    return n*2;
}

long syscall1(){
}


long syscall(long num, long r0, long r1, long r2, long r3){
    register long _r7 asm("r7") = num;
    register long _r3 asm("r3") = r3;
    register long _r2 asm("r2") = r2;
    register long _r1 asm("r1") = r1;
    register long _r0 asm("r0") = r0;

    asm volatile("svc 0x0"
            : "=r"(_r0)
            : "r"(_r0), "r"(_r1), "r"(_r2), 
            "r"(_r3), "r"(_r7)
            : "memory");


    return (long)_r0;
}



long testcall(int n){
    return syscall(0, n, 0, 0, 0);
}


