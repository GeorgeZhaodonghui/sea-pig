#include "adc.h"
#include "../s3c2440_soc.h"

void adc_ready(void)
{
	while (!(ADCCON & (0x1 << 15)));
}

int adc_init(unsigned int channel)
{
	if (channel > 3)
		return -1;

	/* adc control register */
	ADCCON &= ~((0x1 << 14) | (0xff << 6) | (0x7 << 3) | (0x1 << 2) | (0x1 << 0));
	ADCCON |= (0x1 << 14) | (49 << 6) | (channel << 3) | (0x0 << 2);

	/* adc conversion data register */
	ADCDAT0 &= ((0x1 << 14) | (0x3 << 12));

	return 0;
}

void adc_delay(unsigned short delay)
{
	ADCDLY = delay;
}

void adc_enable(void)
{
	/* A/D conversion starts */
	ADCCON |= (1 << 0);
}

int adc_read(unsigned int channel)
{
	unsigned int data;

	adc_init(channel);
	adc_enable();
	adc_ready();
	data = (ADCDAT0 & 0x3ff);

	return data;
}



