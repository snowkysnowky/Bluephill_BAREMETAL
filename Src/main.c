#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "uart.h"
#include "adc.h"

uint16_t adc_value;

int main(void){
    
    
    uart1_tx_init();
    pa1_adc_init();
    
    
    while(1)
    {
        start_conversion();
        adc_value = adc_read();
        printf("ADC Value: %lu\n", adc_value);
    }
}




 