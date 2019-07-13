#include "s3c2440_soc.h"
#include "timer.h"

void delay(volatile int time)
{
	while(time--);
}

int leds(void)
{
	int	i;
	int	val = 0;

	GPFCON	&= ~((3 << 8) | (3 << 10) | (3 << 12));
	GPFCON	|= ((1 << 8) | (1 << 10) | (1 << 12));

	for (i = 0; i < (3 * 8); i++) {
		GPFDAT	|= (7 << 4);
		GPFDAT	&= ~(val << 4);
		val++;
		if (val == 8)
			val = 0;
		delay(100000);
	}

	return 0;
}

static void leds_handle(void)
{
	volatile static int val = 0;
	volatile static int count = 0;

	if (count == 49) {
		GPFDAT	|= (7 << 4);
		GPFDAT	&= ~(val << 4);
		val++;
		count = 0;
	} else {
		count++;
	}
}

int leds_init(void)
{
	GPFCON	&= ~((3 << 8) | (3 << 10) | (3 << 12));
	GPFCON	|= ((1 << 8) | (1 << 10) | (1 << 12));

	GPFDAT |= (7 << 4);
	timer_register("leds", leds_handle);

	return 0;
}

