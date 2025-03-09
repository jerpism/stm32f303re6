#include <common.h>
#include <gpio.h>

struct systick {
    volatile uint32_t CSR;
    volatile uint32_t RVR;
    volatile uint32_t CVR;
    volatile uint32_t CALIB;
};

#define SYSTICK ((struct systick*)0xE000E010)



void systick_init(uint32_t c){
    // 24 bit register
    if((c - 1) > 0xffffff){
        return;
    }

    SYSTICK->RVR = c-1;
    SYSTICK->CVR = 0;
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2); // Enable Systick with processor clock and interrupt
//    RCC->APB2ENR |= BIT(0); // enable SYSCFG clock

}
