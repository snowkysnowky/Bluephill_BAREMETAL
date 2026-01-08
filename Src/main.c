//TODO: Blink an LED
// LED connected to pin PC13
// Steps to implement:
// 1. Enable the GPIOC clock
// 2. Configure PC13 as output
// 3. Toggle PC13 in an infinite loop with a delay
#define STM32F10x
#define PERIPH_BASE           (0x40000000UL)
#define APB2PERIPH_OFFSET     (0x00010000UL)
#define APB2PERIPH_BASE      (PERIPH_BASE + APB2PERIPH_OFFSET)
#define GPIOC_OFFSET         (0x1000UL)
#define GPIOC_BASE          (APB2PERIPH_BASE + GPIOC_OFFSET)
#define AHBPERIPH_OFFSET     (0x00018000UL)
#define AHBPERIPH_BASE      (PERIPH_BASE + AHBPERIPH_OFFSET)
#define RCC_OFFSET           (0x00009000UL)
#define RCC_BASE            (AHBPERIPH_BASE + RCC_OFFSET)

#define AHBENR_OFFSET        (0x14UL)
#define RCC_AHBENR          (*(volatile unsigned int *)(RCC_BASE + AHBENR_OFFSET))

#define APB2ENR_OFFSET        (0x18UL)
#define RCC_APB2ENR         (*(volatile unsigned int *)(RCC_BASE + APB2ENR_OFFSET))

#define GPIOCEN            (1U << 4)

#define CRH_OFFSET          (0x04UL)
#define GPIOC_CRH          (*(volatile unsigned int *)(GPIOC_BASE + CRH_OFFSET))

#define ODR_OFFSET          (0x0CUL)
#define GPIOC_ODR          (*(volatile unsigned int *)(GPIOC_BASE + ODR_OFFSET))

#define PC13               (1U << 13)
#define LED_PIN           PC13


int main(void){
    // 1. Enable the GPIOC clock
    RCC_APB2ENR |= GPIOCEN;
    // 2. set PC13 as output
    GPIOC_CRH = (GPIOC_CRH & ~(0xF << 20 )) | (0x2 << 20); // Output mode, max speed 2 MHz, push-pull

    
    while(1){

        GPIOC_ODR ^= LED_PIN;
        for(int i = 0;i<100000;i++);

    }

}
