#include <gpio.h>
#include <stdint.h>
#include <rcc.h>
#include <common.h>
#include <systick.h>
#include <task.h>
#include <nvic.h>
#include <usart.h>
#include <shell.h>
#include <libc.h>

#include <syscall.h>


void main(void){
    // PendSV to lowest priority
    // SysTick to highest priority
    nvic_set_priority(NVIC_PENDSV, 0xff);
    nvic_set_priority(NVIC_SVCALL, 0xff);
    nvic_set_priority(NVIC_SYSTICK, 0x04);

    uart_init(9600);
    systick_init(8000000 / 100);
    init_led();

    uint32_t *pstack = malloc(sizeof(uint32_t) * 128);
    struct task *shellt = create_task(&shell, pstack, 128, "shell");
    sched_add(shellt);

    // Jump into the scheduler, hopefully to never return
    sched_start();

    for(;;);

    return;
}

__attribute__((naked, noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();             
    for (;;) (void) 0;  // Infinite loop in the case if main() returns
}
extern void _estack(void);  
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern void SVCall_Handler(void);

void HardFault_Handler(void){
    while(1);
}

__attribute__((section(".vectors"))) void (*const tab[16 + 84])(void) = {
    _estack, _reset, 0, HardFault_Handler, 0, 0, 0, 0, 0, 0, 0, SVCall_Handler, 0, 0, PendSV_Handler, SysTick_Handler
};
