#ifndef _ADC_H
#define _ADC_H

void adc_ready(void);
int adc_init(void);
void adc_delay(unsigned short delay);
void adc_enable(void);
int adc_read(void);

#endif


