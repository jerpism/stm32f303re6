#include <debug.h>
#include <usart.h>
#include <stdint.h>
#include <libc.h>


/* TODO: maybe have some clever macro system for this
 * or not. Probably not. */
void dbg_print1(const char *s, enum base base, uint32_t a){
    char tmp[11];
    uart_sendstr(s);
    itoa(a, tmp, base);
    uart_sendstr(tmp);

}

void dbg_print2(const char *s, enum base base, uint32_t a, uint32_t b){
    char tmp[11];
    uart_sendstr(s);
    itoa(a, tmp, base);
    uart_sendstr(tmp);

    itoa(b, tmp, base);
    uart_sendstr(tmp);
}

void dbg_print3(const char *s, enum base base, uint32_t a, uint32_t b, uint32_t c){
    char tmp[11];
    uart_sendstr(s);
    itoa(a, tmp, base);
    uart_sendstr(tmp);

    itoa(b, tmp, base);
    uart_sendstr(tmp);

    itoa(c, tmp, base);
    uart_sendstr(tmp);
}



