#include <stdint.h>
#include <common.h>
#include <rcc.h>
#include <gpio.h>
#include <usart.h>
#include <dma.h>

#define FREQ 8000000

void uart_init(uint32_t baud) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_DMA1EN;
    RCC->CFGR3 |= RCC_CFGR3_USART2SW_HSI;

    GPIO(0)->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));
    GPIO(0)->AFR[0] |= (7 << (2 * 4)) | (7 << (3 * 4));

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    USART2->CR1 = 0;
    USART2->BRR = FREQ / baud;
    USART2->CR3 |= USART_CR3_DMAT;
    USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

void uart_send(uint8_t data) {
    while (!(USART2->ISR & USART_ISR_TXE)) spin(1);
    USART2->TDR = data;
}

void uart_sendstr(const char *s) {
    while (*s) {
        uart_send(*s++);
    }
}

void uart_receive(uint8_t *buffer, uint32_t len) {
    DMA1_Channel6->CCR = 0;
    DMA1_Channel6->CPAR = (uint32_t)&USART2->RDR;
    DMA1_Channel6->CMAR = (uint32_t)buffer;
    DMA1_Channel6->CNDTR = len;

    DMA1_Channel6->CCR =
        DMA_CCR_MINC |
        DMA_CCR_DIR  |
        DMA_CCR_EN;

    while (!(DMA1->ISR & DMA_ISR_TCIF6));
    DMA1->IFCR |= DMA_IFCR_CTCIF6;
}

void uart_sendstr_dma(const char *s) {
    uint32_t len = 0;
    while (s[len]) len++;

    DMA1_Channel7->CCR = 0;

    DMA1_Channel7->CPAR = (uint32_t)&USART2->TDR;
    DMA1_Channel7->CMAR = (uint32_t)s;
    DMA1_Channel7->CNDTR = len;

    DMA1_Channel7->CCR =
        DMA_CCR_MINC |
        DMA_CCR_DIR  |
        DMA_CCR_TCIE |
        DMA_CCR_EN;

    while (!(DMA1->ISR & DMA_ISR_TCIF7));
    DMA1->IFCR |= DMA_IFCR_CTCIF7;
}
