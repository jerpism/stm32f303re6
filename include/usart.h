#ifndef USART_H_
#define USART_H_
#include <stdint.h>

/* Just USART for now, not sure if UART differs */
struct usart{
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
#define USART1 ((struct usart*) 0x40013800)
#define USART2 ((struct usart*) 0x40004400)
#define USART3 ((struct usart*) 0x40004800)

/* CR1 */
#define USART_CR1_UE        (1UL << 0)
#define USART_CR1_UESM      (1UL << 1)
#define USART_CR1_RE        (1UL << 2)
#define USART_CR1_TE        (1UL << 3)
#define USART_CR1_IDLEIE    (1UL << 4)
#define USART_CR1_RXNEIE    (1UL << 5)
#define USART_CR1_TCIE      (1UL << 6)
#define USART_CR1_TXIE      (1UL << 7)
#define USART_CR1_PEIE      (1UL << 8)
#define USART_CR1_PS        (1UL << 9)
#define USART_CR1_PCE       (1UL << 10)
#define USART_CR1_WAKE      (1UL << 11)
#define USART_CR1_M0        (1UL << 12)
#define USART_CR1_MME       (1UL << 13)
#define USART_CR1_CMIE      (1UL << 14)
#define USART_CR1_OVER8     (1UL << 15)

/*TODO: implement something for these
 * #define USART_CR1_DEDT      (1UL << 1)
 * #define USART_CR1_DEAT      (1UL << 0)
 * */

#define USART_CR1_RTOIE     (1UL << 26)
#define USART_CR1_EOBIE     (1UL << 27)
#define USART_CR1_M1        (1UL << 28)

/* ISR */
#define USART_ISR_PE        (1UL << 0)
#define USART_ISR_FE        (1UL << 1)
#define USART_ISR_NF        (1UL << 2)
#define USART_ISR_ORE       (1UL << 3)
#define USART_ISR_IDLE      (1UL << 4)
#define USART_ISR_RXNE      (1UL << 5)
#define USART_ISR_TC        (1UL << 6)
#define USART_ISR_TXE       (1UL << 7)
#define USART_ISR_LBDF      (1UL << 8)
#define USART_ISR_CTSIF     (1UL << 9)
#define USART_ISR_CTS       (1UL << 10)
#define USART_ISR_RTOF      (1UL << 11)
#define USART_ISR_EOBF      (1UL << 12)
#define USART_ISR_ABRE      (1UL << 14)
#define USART_ISR_ABRF      (1UL << 15)
#define USART_ISR_BUSY      (1UL << 16)
#define USART_ISR_CMF       (1UL << 17)
#define USART_ISR_SBKF      (1UL << 18)
#define USART_ISR_RWU       (1UL << 19)
#define USART_ISR_WUF       (1UL << 20)
#define USART_ISR_TEACK     (1UL << 21)
#define USART_ISR_REACK     (1UL << 22)

void uart_init(uint32_t baud);
void uart_send(uint8_t data);
void uart_sendstr(const char *s);
void uart_receive(uint8_t *buffer, uint32_t len);
void uart_sendstr_dma(const char *s);


#endif /* USART_H_ */
