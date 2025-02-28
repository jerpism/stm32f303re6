#include <stdint.h>
#include <common.h>

struct rcc{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t AHBRSTR;
    volatile uint32_t CFGR2;
    volatile uint32_t CFGR3;
};
#define RCC ((struct rcc*) 0x40021000)

// Bit positions for peripherals in EN/RST registers
enum rcc_apb1{
    TIM2    = 0,
    TIM3    = 1,
    TIM4    = 2,
    TIM6    = 4,
    TIM7    = 5,
    WWDG    = 11,
    SPI2    = 14,
    SPI3    = 15,
    USART2  = 17,
    USART3  = 18,
    UART4   = 19,
    UART5   = 20,
    I2C1    = 21,
    I2C2    = 22,
    USB     = 23,
    CAN     = 25,
    DAC2    = 26,
    PWR     = 28,
    DAC1    = 29,
    I2C3    = 30
};

enum rcc_apb2{
    SYSCFG  = 0,
    TIM1    = 11,
    SPI1    = 12,
    TIM8    = 13,
    USART1  = 14,
    SPI4R   = 15,
    TIM15   = 16,
    TIM16   = 17,
    TIM17   = 18,
    TIM20   = 20
};

void test_enable_rcc(){
    RCC->CFGR3 |= BIT(17) | BIT(16); // set USART2 clock source to HSI 8MHz
    RCC->APB1ENR |= BIT(17); // USART2EN
    RCC->AHBENR |= BIT(17);  // GPIOAEN
}
