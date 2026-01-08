#include <stdint.h>

#define STM32F10x
#define __IO volatile

#define PERIPH_BASE           (0x40000000UL)
#define APB2PERIPH_OFFSET     (0x00010000UL)
#define APB2PERIPH_BASE      (PERIPH_BASE + APB2PERIPH_OFFSET)
#define GPIOC_OFFSET         (0x1000UL)
#define GPIOC_BASE          (APB2PERIPH_BASE + GPIOC_OFFSET)
#define AHBPERIPH_OFFSET     (0x00018000UL)
#define AHBPERIPH_BASE      (PERIPH_BASE + AHBPERIPH_OFFSET)
#define RCC_OFFSET           (0x00009000UL)
#define RCC_BASE            (AHBPERIPH_BASE + RCC_OFFSET)
#define GPIOCEN            (1U << 4)  // Bit 4: IO port C clock enable

#define PC13               (1U << 13)
#define LED_PIN           PC13

typedef struct{
    __IO uint32_t CR;       // Clock control register Address offset: 0x00
    __IO uint32_t CFGR;     // Clock configuration register Address offset: 0x04
    __IO uint32_t CIR;      // Clock interrupt register Address offset: 0x08
    __IO uint32_t APB2RSTR; // APB2 peripheral reset register Address offset: 0x0C
    __IO uint32_t APB1RSTR; // APB1 peripheral reset register Address offset: 0x10
    __IO uint32_t AHBENR;   // AHB peripheral clock enable register Address offset: 0x14
    __IO uint32_t APB2ENR;  // APB2 peripheral clock enable register Address offset: 0x18
    __IO uint32_t APB1ENR;  // APB1 peripheral clock enable register Address offset: 0x1C
    __IO uint32_t BDCR;     // Backup domain control register Address offset: 0x20
    __IO uint32_t CSR;      // Control/status register Address offset: 0x24
    __IO uint32_t AHBSTR;   // AHB peripheral reset register Address offset: 0x28
    __IO uint32_t CFGR2;    // Clock configuration register 2 Address offset: 0x2C

} RCC_TypeDef;

typedef struct{
    __IO uint32_t CRL;      // Port configuration register low Address offset: 0x00
    __IO uint32_t CRH;      // Port configuration register high Address offset: 0x04
    __IO uint32_t IDR;      // Port input data register Address offset: 0x08
    __IO uint32_t ODR;      // Port output data register Address offset: 0x0C
    __IO uint32_t BSRR;     // Port bit set/reset register Address offset: 0x10
    __IO uint32_t BRR;      // Port bit reset register Address offset: 0x14
    __IO uint32_t LCKR;     // Port configuration lock register Address offset: 0x18

} GPIO_TypeDef;

#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)


int main(void){
    // 1. Enable the GPIOC clock
    // RCC_APB2ENR |= GPIOCEN;
    RCC->APB2ENR |= GPIOCEN;
    // 2. set PC13 as output
    // GPIOC_CRH = (GPIOC_CRH & ~(0xF << 20 )) | (0x2 << 20); // Output mode, max speed 2 MHz, push-pull
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20 )) | (0x2 << 20); // Output mode, max speed 2 MHz, push-pull

    
    while(1){
        // 3. Toggle PC13 in an infinite loop with a delay
        // GPIOC_ODR ^= LED_PIN;
        GPIOC->ODR ^= LED_PIN;
        for(int i = 0;i<100000;i++);

    }

}
