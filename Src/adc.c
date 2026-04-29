#include "stm32f103xb.h"
#include "adc.h"


void pa1_adc_init(void)
{
    //configure ADC GPIO pin
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable clock for GPIOA

    //set PA1 as analog input
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1); // Clear MODE1 and CNF1 bits
    GPIOA->CRL |= GPIO_CRL_CNF1_1; // Set CNF1 to 10 (Analog input)

    //configure ADC module
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Enable clock for ADC1
    
    // ADC conversion seq
    // ADC1->SQR3 = 0; // Clear SQR3
    ADC1->SQR3 |= 1; // Set channel 1 (PA1) as the first conversion in regular sequence

    //Conversion sequence length
    ADC1->SQR1 = 0x00; // Set sequence length to 1 conversion

    //Enable ADC module
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC1
}

void start_conversion(void)
{
    ADC1->CR2 |= ADC_CR2_ADON; // Trigger conversion by writing ADON again (STM32F1 quirk)
}

uint16_t adc_read(void)
{
    while (!(ADC1->SR & ADC_SR_EOC)){} // Wait for conversion to complete
    return ADC1->DR; // Return the converted value
}