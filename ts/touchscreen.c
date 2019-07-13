#include "touchscreen.h"
#include "adc.h"
#include "../s3c2440_soc.h"
#include "../exception.h"
#include "../my_printf.h"
#include "ts_test.h"

#define	ADC_SUBIRQ				(1 << 10)
#define	TC_SUBIRQ					(1 << 9)

volatile static unsigned int timer_lock;
volatile static unsigned int down_lock;

static xy_buf xy_position;
volatile static unsigned int count;
volatile static unsigned char xy_position_flag;
volatile unsigned int final_count;

static void touchscreen_wait_down_interrupt(void)
{
	/* adc touch screen control register */
	ADCTSC = STYLUS_DOWN_INT | YM_ENABLE | YP_DISABLE | XM_DISABLE |\
				XP_DISABLE | PULL_UP_ENABLE | NORMAL_PST | WAIT_INTERRUPT;
}

static void touchscreen_wait_up_interrupt(void)
{
	/* adc touch screen control register */
	ADCTSC = STYLUS_UP_INT | YM_ENABLE | YP_DISABLE | XM_DISABLE |\
				XP_DISABLE | PULL_UP_ENABLE | NORMAL_PST | WAIT_INTERRUPT;
}

static void touchscreen_auto_measure(void)
{
	/* adc touch screen control register */
	ADCTSC = AUTO_PST | NO_OPERATION;
}

void read_xy_position_flag(volatile unsigned char *flag)
{
	*flag = xy_position_flag;
}

void deliver_xy_buf(unsigned int *px_position, unsigned int *py_position)
{
	unsigned int i;
	unsigned int x_position = 0;
	unsigned int y_position = 0;
	
	for (i = 0; i < 5; i++) {
		x_position += xy_position.x_buf[i];
		y_position += xy_position.y_buf[i];
	}
	*px_position = x_position / 5;
	*py_position = y_position / 5;
	xy_position_flag = 0;
}

void read_pressure_flag(volatile unsigned char *flag)
{
	*flag = down_lock;
}

static void adc_handle(void)
{
	unsigned int x_position;
	unsigned int y_position;
	unsigned int final_x = 0;
	unsigned int final_y = 0;
	unsigned int i;

	x_position = ADCDAT0;
	y_position = ADCDAT1;

	if (down_lock) {
		x_position &= 0x3ff;
		y_position &= 0x3ff;
		if (!whether_finish()) {
			xy_position.x_buf[final_count] = x_position;
			xy_position.y_buf[final_count] = y_position;
			printf("x_pos = %d, y_pos = %d, count = %d\n\r", xy_position.x_buf[final_count], xy_position.y_buf[final_count], final_count);
			final_count++;
			if (final_count >= 5) {
				for (i = 0; i < 5; i++) {
					final_x += xy_position.x_buf[i];
					final_y += xy_position.y_buf[i];
				}
				final_x = final_x / 5;
				final_y = final_y / 5;
				draw(final_x, final_y);
				printf("x_position = %d, y_position = %d\n\r", final_x, final_y);
				final_x = 0;
				final_y = 0;
				final_count = 0;
			}
		}
		if (!xy_position_flag) {
			xy_position.x_buf[count] = x_position;
			xy_position.y_buf[count] = y_position;
			count++;
			
			if (count == 5) {
				xy_position_flag = 1;
				count = 0;
				timer_lock = 0;
				touchscreen_wait_up_interrupt();

				return ;
			}
		}
		
		timer_lock = 1;
		touchscreen_wait_up_interrupt();
	} else {
		timer_lock = 0;
		final_count = 0;
		touchscreen_wait_down_interrupt();
	}
}

static void tc_handle(void)
{
	if (ADCDAT0 & (0x1 << 15)) {
		touchscreen_wait_down_interrupt();
		down_lock = 0;
	} else {
		touchscreen_auto_measure();
		adc_enable();
		down_lock = 1;
	}
	timer_lock = 0;
}

static void tc_adc_handle(unsigned int irq)
{
	unsigned int subirq;

	subirq = SUBSRCPND;
	
	if (subirq & ADC_SUBIRQ) {
		adc_handle();
		SUBSRCPND |= ADC_SUBIRQ;
	}
	
	if (subirq & TC_SUBIRQ) {
		tc_handle();
		SUBSRCPND |= TC_SUBIRQ;
	}
}

static void touchscreen_timer_handle(void)
{
	if (!timer_lock)
		return ;
	if (!down_lock) {
		timer_lock = 0;
		touchscreen_wait_down_interrupt();
		return ;
	}
	touchscreen_auto_measure();
	adc_enable();
}

void touchscreen_init(void)
{
	/* adc control register */
	ADCCON &= ~((0x1 << 14) | (0xff << 6) | (0x7 << 3) | (0x1 << 2) | (0x1 << 0));
	ADCCON |= (0x1 << 14) | (49 << 6) | (0x0 << 3) | (0x0 << 2);

	touchscreen_wait_down_interrupt();
	timer_lock = 0;
	down_lock = 0;
	count = 0;
	final_count = 0;
	xy_position_flag = 0;
	timer_register("touchscreen", touchscreen_timer_handle);

	/* 1ms delay */
	adc_delay(0x2ee0);
}

void tc_adc_interrupt_init(void)
{
	/* sub source pending register */
	SUBSRCPND |= ADC_SUBIRQ | TC_SUBIRQ;
	/* interrupt sub mask register */
	INTSUBMSK &= ~(ADC_SUBIRQ | TC_SUBIRQ);

	/* register irq */
	interrupt_register(tc_adc_handle, 31);
}


