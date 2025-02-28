#include <stdint.h>
#include <common.h>

#define GPIO(bank) ((struct gpio*) (0x48000000 + (0x400 * bank)))

struct gpio{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
};


enum gpio_mode{
    GPIO_MODE_INPUT     = 0,
    GPIO_MODE_OUTPUT    = 1,
    GPIO_MODE_AF        = 2,
    GPIO_MODE_ANALOG    = 3
};

enum gpio_af{
    GPIO_AF0   = 0,
    GPIO_AF1   = 1,
    GPIO_AF2   = 2,
    GPIO_AF3   = 3,
    GPIO_AF4   = 4,
    GPIO_AF5   = 5,
    GPIO_AF6   = 6,
    GPIO_AF7   = 7,
    GPIO_AF8   = 8,
    GPIO_AF9   = 9,
    GPIO_AF10  = 10,
    GPIO_AF11  = 11,
    GPIO_AF12  = 12,
    GPIO_AF13  = 13,
    GPIO_AF14  = 14,
    GPIO_AF15  = 15,
};


void gpio_set_mode(struct gpio *gpio, uint8_t pin, enum gpio_mode mode){
    gpio->MODER &= ~(3UL << (pin * 2)); // clear
    gpio->MODER |= (mode << (pin * 2)); // and set
}

void gpio_set_af(struct gpio *gpio, uint8_t pin, enum gpio_af af){
  //  int afr;
//    afr = (pin < 8) ?  0 :  1; 
 //   gpio->AFR[afr] = af << (pin * 4);
        gpio->AFR[0] = af << (pin * 4);
}

void test_usart_set_pins(){
    // Set PA2 and PA3 (USART2) pins to AF
    gpio_set_mode(GPIO(0), 2, GPIO_MODE_AF);
    gpio_set_mode(GPIO(0), 3, GPIO_MODE_AF);
 
    // and select AF7 (USART2 TX AND RX)
    gpio_set_af(GPIO(0), 2, GPIO_AF7);
    gpio_set_af(GPIO(0), 3, GPIO_AF7);
}

void led_on(){
    GPIO(0)->ODR |= BIT(5);
}

void led_off(){
    GPIO(0)->ODR &= ~BIT(5);
}

void init_led(){
    gpio_set_mode(GPIO(0), 5, GPIO_MODE_OUTPUT);
}




