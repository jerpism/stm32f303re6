#include <stdint.h>
#include <common.h>
#include <rcc.h>
#include <gpio.h>
#include <usart.h>

/* Since USART2 is the one connected to the onboard ST-Link and at this point the only thing we care about is getting serial output to a computer
 * this will only take into consideration enabling and controlling USART2 */
#define FREQ 8000000

void uart_init(uint32_t baud){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->CFGR3 |= RCC_CFGR3_USART2SW_HSI;

    //PA2 and PA3 for USART2
    GPIO(0)->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));
    GPIO(0)->AFR[0] |= (7 << (2 * 4)) | (7 << (3 * 4));

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;


    USART2->CR1 = 0; // Disable
    USART2->BRR = FREQ / baud; // set baudrate, assuming OVER8 = 0
    USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

void uart_sendstr(const char *s){
    while(*s){
        uart_send(*s++);
    }
}
void uart_send(uint8_t data){
    while(!(USART2->ISR & USART_ISR_TXE) ) spin(1); // Block until TXE bit is set
    USART2->TDR = (uint8_t)data;
}

uint8_t uart_read(){
    while(!(USART2->ISR & USART_ISR_RXNE)) spin(1);
    return (uint8_t)USART2->RDR;
}


