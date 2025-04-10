#include <rcc.h>
#include <gpio.h>
#include <stdint.h>

#include <usart_mr.h>

#define USART_BAUDRATE 9600
#define APB2_CLOCK 8000000

#define RX_BUFFER_SIZE 64
volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

void USART1_Init(void) {
    // Otetaan GPIOA käyttöön (USART1 TX: PA9, RX: PA10)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIO(0)->MODER |= (2 << (9 * 2)) | (2 << (10 * 2)); // PA9 & PA10 AF-moodiin
    GPIO(0)->AFR[1] |= (7 << (1 * 4)) | (7 << (2 * 4)); // AF7 (USART1)

    // Otetaan USART1 käyttöön
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    
    // Asetetaan baudinopeus
    USART1->BRR = APB2_CLOCK / USART_BAUDRATE;
    
    // Otetaan vastaanotto, lähetys ja vastaanottokeskeytys käyttöön
    USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;

/*   JM: pitää vielä implementoida tämä
 *
    // Otetaan käyttöön NVIC-keskeytys
    NVIC_EnableIRQ(USART1_IRQn);

*/
}

void USART1_SendChar(char c) {
    while (!(USART1->ISR & USART_ISR_TXE));
    USART1->TDR = c;
}

void USART1_SendString(const char *str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}

char USART1_ReadChar(void) {
    if (rx_head == rx_tail) return 0;
    char c = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
    return c;
}

/*void USART1_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_RXNE) {
        char c = USART1->RDR;
        uint8_t next_head = (rx_head + 1) % RX_BUFFER_SIZE;
        if (next_head != rx_tail) {
            rx_buffer[rx_head] = c;
            rx_head = next_head;
        }
    }
}*/

/*int main(void) {
    USART1_Init(); // Alustetaan USART1
    USART1_SendString("Kirjoita jotain: \n");
    
    char input_buffer[RX_BUFFER_SIZE];
    uint8_t index = 0;
    
    while (1) {
        char c = USART1_ReadChar();
        if (c) { 
            if (c == '\n' || c == '\r') {
                input_buffer[index] = '\0';
                USART1_SendString("Kirjoitit: ");
                USART1_SendString(input_buffer);
                USART1_SendString("\n");
                index = 0; //
            } else if (index < RX_BUFFER_SIZE - 1) {
                input_buffer[index++] = c;
            }
        }
    }
}*/
