#include <stdint.h>
#include <common.h>
#include <gpio.h>

#define GPIO(bank) ((struct gpio*) (0x48000000 + (0x400 * bank)))


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




