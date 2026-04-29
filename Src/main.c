#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "uart.h"
#include "adc.h"

uint16_t adc_value;

int main(void){
    
    
    uart1_tx_init();
    pa1_adc_init();
    start_conversion();
    
    while(1)
    {
        
        adc_value = adc_read();
        printf("ADC Value: %d\r\n", adc_value);
    }
}




 