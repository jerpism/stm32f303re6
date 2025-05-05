#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

#define BIT(x) (1UL << (x))

static inline void spin(volatile uint32_t c){
    while(c--)(void)0;
}

static inline void disable_irq(){
/*    asm volatile("mov r0, #0\n\t"
                 "svc 0");*/
}

static inline void enable_irq(){
/*    asm volatile("mov r0, #1\n\t"
                 "svc 0");*/
}

#endif /* COMMON_H_ */
