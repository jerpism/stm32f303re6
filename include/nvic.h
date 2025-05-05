#ifndef NVIC_H_
#define NVIC_H_

#include <stdint.h>

#define NVIC_IMPLEMENTED 100

#define NVIC_SVCALL     11
#define NVIC_PENDSV     14
#define NVIC_SYSTICK    15

#define NVIC_IPR_BASE   (0xE000E400)


void nvic_set_priority(unsigned int n, uint8_t prio);


#endif /* NVIC_H_ */
