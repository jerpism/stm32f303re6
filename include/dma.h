#ifndef DMA_H
#define DMA_H

#include <stdint.h>

#define DMA1_BASE (0x40020000UL)
#define DMA1 ((DMA_TypeDef *)DMA1_BASE)

typedef struct {
    volatile uint32_t CCR;
    volatile uint32_t CNDTR;
    volatile uint32_t CPAR;
    volatile uint32_t CMAR;
    volatile uint32_t RESERVED[2];
} DMA_Channel_TypeDef;

typedef struct {
    volatile uint32_t ISR;
    volatile uint32_t IFCR;
    DMA_Channel_TypeDef CHANNEL[7];
} DMA_TypeDef;

#define DMA_ISR_TCIF6 (1 << 20)
#define DMA_ISR_TCIF7 (1 << 22)

#define DMA_IFCR_CTCIF6 (1 << 20)
#define DMA_IFCR_CTCIF7 (1 << 22)

#define DMA_CCR_MINC  (1 << 7)
#define DMA_CCR_DIR    (1 << 4)
#define DMA_CCR_TCIE   (1 << 1)
#define DMA_CCR_EN     (1 << 0)

#define RCC_AHBENR_DMA1EN (1 << 21)

#endif