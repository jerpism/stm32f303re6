#ifndef USART_H_
#define USART_H_

#include <stdint.h>
void uart_init(uint32_t baud);
void uart_send(uint8_t data);

#endif /* USART_H_ */
