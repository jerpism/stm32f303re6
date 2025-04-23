#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

#define BIT(x) (1UL << (x))

static inline void spin(volatile uint32_t c){
    while(c--)(void)0;
}

static inline void disable_irq(){
    asm volatile("cpsid if");
}

static inline void enable_irq(){
    asm volatile("cpsie if");
}

#endif /* COMMON_H_ */
