#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "uart.h"

#define GPIOCEN     (1<<4)
#define GPIOC_13    (1<<13)
#define LED_PIN     GPIOC_13

char key;
 

int main(void){

    /*Enable clock access to GPIOC*/
    RCC->APB2ENR |= GPIOCEN;

    /*Set PC13 as output pin*/
    GPIOC->CRH &= ~(0xF << 20); // Clear the bits for PC13
    GPIOC->CRH |= (0x1 << 20); // Set the bits for PC13 to output mode (0b0001)
    GPIOC->ODR |= LED_PIN;   // HIGH → LED OFF

    uart1_rxtx_init();
    
    while(1)
    {
        key = uart1_read();
        if(key == '1')
        {
        GPIOC->ODR &= ~LED_PIN;  // LOW → LED ON
        }
        else if(key == '0')
        {
        GPIOC->ODR |= LED_PIN;   // HIGH → LED OFF
        }
}
}




 