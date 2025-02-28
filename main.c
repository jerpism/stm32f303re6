#include <gpio.h>
#include <stdint.h>
#include <rcc.h>
#include <usart.h>
#include <common.h>

void main(void) {
    test_enable_rcc();
    init_led();
    test_usart_set_pins();
    uart_init(9600);

    for(;;){
        uart_send('A');
//        spin(99);
//        led_on();
//        spin(999999);
//        led_off();
        spin(999999);

    }

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
    _estack, _reset
};
