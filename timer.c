#include "s3c2440_soc.h"
#include "timer.h"
#include "exception.h"
#include "string_utils.h"

static timer_desc timer_array[TIMER_NUM];

int timer_register(const unsigned char *name, timer_handle_func timer_handle_fun)
{
	unsigned int i;

	for (i = 0; i < TIMER_NUM; i++) {
		if (timer_array[i].timer_handle_fun == NULL) {
			timer_array[i].name				= name;
			timer_array[i].timer_handle_fun		= timer_handle_fun;
			return 0;
		}
	}
	return -1;
}

int timer_unregister(const unsigned char *name)
{
	unsigned int i;

	for (i = 0; i < TIMER_NUM; i++) {
		if (!strcmp(name, timer_array[i].name)) {
			timer_array[i].name				= NULL;
			timer_array[i].timer_handle_fun		= NULL;
			return 0;
		}
	}
	return -1;
}

static void timer_handle(unsigned int unused)
{
	unsigned int i;

	for (i = 0; i < TIMER_NUM; i++) {
		if (timer_array[i].timer_handle_fun)
			timer_array[i].timer_handle_fun();
	}
}

/* 10ms */
void timer_init(void)
{
	/* timer configuration register0: {prescaler} = 24 5 */
	TCFG0	&= ~(0xff);
	TCFG0	|= 0x4;
	/* timer configuration register1: {divider} = 16 2 */
	TCFG1	&= ~(0xf);
	TCFG1	|= 0x0;
	/* timer0 count buffer register & compare buffer register */
	TCNTB0	&= ~(0xffff);
	TCNTB0	|= 0xea60;
	TCMPB0	&= ~(0xffff);
	TCMPB0	|= 0x7530;
	/* timer control register: timer0 */
	TCON	|= (1 << 1);
	TCON	&= ~(1 << 1);
	
	TCON	|= ((1 << 0) | (1 << 3));

	interrupt_register(timer_handle, 10);
}


