#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define GPIOCEN (1 << 4)
#define GPIOC13 (1 << 13)

#define LED_PIN GPIOC13


int main(void){
    
    RCC->APB2ENR |= GPIOCEN; // Enable clock for GPIOC
    // GPIOC->CRH &= ~(0xF << 20); // Clear the bits for PC13
    // GPIOC->CRH |= (0x3 << 20); // Set PC13 as 50MHz push-pull output
    GPIOC->CRH |= (1U << 20);// Set PC13 as output (0b0011) - 50MHz push-pull
    GPIOC->CRH &= ~(1U << 21);// Clear the bit to ensure it's push-pull


    
    uart1_tx_init();
    
    while(1)
    {
        printf("Second has passed...\n\r");
        GPIOC->ODR ^= LED_PIN; // Toggle LED
        systickDelayMs(1000); // Delay for 1 second
    }
}




 