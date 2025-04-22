#include <stdint.h>
#include <usart.h>
#include <libc.h>
#include <task.h>

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

int strcmp_hack(uint8_t *s1, uint8_t *s2){
    uint8_t c1, c2;
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


void parse_cmd(uint8_t *cmd, size_t n){
    for(size_t i = 0; i < n; ++i){
        if(cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\r'){
            cmd[i] = '\0';
        }
    }

    if(strcmp_hack(cmd, "blink") == 0){
        task_enable(0);
    }else if(strcmp_hack(cmd, "noblink") == 0){
        task_disable(0);
    }else if(strcmp_hack(cmd, "ledon") == 0){

    }else if(strcmp_hack(cmd, "ledoff") == 0){

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
