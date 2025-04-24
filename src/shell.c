#include <stdint.h>
#include <usart.h>
#include <libc.h>
#include <task.h>
#include <gpio.h>

const char *prompt = "\r~$ ";

void memset_hack(void *dst, size_t bytes, uint8_t val){
    for(size_t i = 0; i < bytes; ++i){
        ((uint8_t*)dst)[i] = 0;
    }
}

void memmove_hack(void *dst, void *src, size_t n){
    // Please call responsibly with reasonable size values
    uint8_t tmp[n];

    for(size_t i = 0; i < n; ++i){
        tmp[i] = ((uint8_t*)src)[i];
    }

    for(size_t i = 0; i < n; ++i){
        ((uint8_t*)dst)[i] = tmp[i];
    }
}

int strcmp_hack(const char *str1, const char *str2){
    char *s1 = str1;
    char *s2 = str2;
    char c1, c2;
    do{
        c1 = *s1++;
        c2 = *s2++;
        if(c1 == '\0')
            return c1 - c2;
    }while(c1 == c2);

    return c1 - c2;
}


size_t shell_readinput(uint8_t *buff, size_t size){
    /* Reads into buffer and echos back input */
    for(size_t i = 0; i < size; ++i){
        buff[i] = uart_read();

        if(buff[i] == '\r'){
            /* This is just temporary for now but the buffers we are getting will always have
             * extra space left over for a NUL and a newline so this won't access out of bounds */
            buff[i+1] = '\n';

            /* Automatically start a new line after we've hit enter */
            uart_sendstr("\r\n");

            return i+1;
        }

        uart_send(buff[i]);
    }

    return size;
}

void test(){
    for(int i = 0; i < 31; ++i){
        uart_send('=');
    }
    uart_sendstr("\n\r");

    uart_sendstr("TTTTTT  EEEEEE   SSSSSS  TTTTTT\r\n");
    uart_sendstr("  TT    EE       SS        TT  \r\n");
    uart_sendstr("  TT    EE       SS        TT  \r\n");
    uart_sendstr("  TT    EEEE     SSSSSS    TT  \r\n");
    uart_sendstr("  TT    EE           SS    TT  \r\n");
    uart_sendstr("  TT    EE           SS    TT  \r\n");
    uart_sendstr("  TT    EEEEEE   SSSSSS    TT  \r\n");

    for(int i = 0; i < 31; ++i){
        uart_send('=');
    }

    uart_sendstr("\n\r");
}


void parse_cmd(uint8_t *cmd, size_t n){
    for(size_t i = 0; i < n; ++i){
        if(cmd[i] == '\n' || cmd[i] == '\r'){
            cmd[i] = '\0';
        }
    }

    if(strcmp_hack(cmd, "test") == 0){
        test();
    }else if(strcmp_hack(cmd, "ps") == 0){
        ps();
    }else if(strcmp_hack(cmd, "spawn") == 0){
        spawn();
    }else if(strcmp_hack(cmd, "kill") == 0){
        kill(1);
    }else if(strcmp_hack(cmd, "ledon") == 0){
        led_on();
    }else if(strcmp_hack(cmd, "ledoff") == 0){
        led_off();
    }

}

void shell(void){
    uint8_t buff[128];
    memset_hack(buff, 128, 0);

    size_t ret = 0;

    while(1){
        uart_sendstr(prompt);
        // Just read -2 and leave last byte as a 0
        // This is just to automatically NUL terminate and leave space for \n in buff
        ret = shell_readinput(buff, (sizeof(buff) / sizeof(buff[0]))-2);
        parse_cmd(buff, ret);
        memset_hack(buff, 128, 0);
    }

}
