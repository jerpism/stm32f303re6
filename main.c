#include <gpio.h>
#include <stdint.h>
#include <rcc.h>
#include <usart.h>
#include <common.h>
#include <systick.h>

static int ledstate = 0;
void SysTick_Handler(void){
    if(ledstate){
        led_off();
        ledstate = 0;
    }else{
        led_on();
        ledstate = 1;
    }
}

void main(void) {
    test_enable_rcc();
    init_led();
    systick_init(8000000 / 2); // tick once every second

    for(;;);

    return;
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
    // memset .bss to zero, and copy .data section to RAM region
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();             // Call main()
    for (;;) (void) 0;  // Infinite loop in the case if main() returns
}
extern void _estack(void);  // Defined in link.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler
};
