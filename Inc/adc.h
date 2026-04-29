#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdio.h>
void pa1_adc_init(void);
void start_conversion(void);
uint16_t adc_read(void);

#endif /* ADC_H */

