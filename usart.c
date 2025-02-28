#include <stdint.h>
#include <common.h>

/* Since USART2 is the one connected to the onboard ST-Link and at this point the only thing we care about is getting serial output to a computer
 * this will only take into consideration enabling and controlling USART2 */
#define FREQ 8000000

struct uart{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
};
#define USART2 ((struct uart*) 0x40004400)


void uart_init(uint32_t baud){
    USART2->CR1 = 0; // Disable
    USART2->BRR = FREQ / baud; // set baudrate, assuming OVER8 = 0
    USART2->CR1 = BIT(3); // Set TE
    USART2->CR1 |= BIT(0); // Set UE

}

void uart_send(uint8_t data){
    while((USART2->ISR & BIT(6)) != 0) spin(1); // Block until TXE bit is set
    USART2->TDR = (uint8_t)data;
}

uint8_t uart_read(){
    return (uint8_t)USART2->RDR;
}


