#include "ts_test.h"
#include "touchscreen.h"
#include "adc.h"
#include "../lcd/framebuffer.h"
#include "../lcd/lcd.h"
#include "tslib.h"

static plcd_operations papplcd_oper;
static p_app_params p_params;

static unsigned int x_position[5];
static unsigned int y_position[5];

volatile unsigned char unfinish_lock = 1;

static int draw_init(void)
{	
	papplcd_oper = lcd_hardware();
	lcd_parameters_deliver(p_params);
	
	return 0;
}

static void wait_position(unsigned int count)
{
	volatile unsigned char flag_data = 0;
	volatile unsigned char flag_down = 1;

	while (1) {
		read_xy_position_flag(&flag_data);
		if (flag_data)
			break ;
	}
	
	printf("hello\n\r");
	deliver_xy_buf(&x_position[count], &y_position[count]);
	printf("x = %d, y= %d\n\r", x_position[count], y_position[count]);

	while (1) {
		read_pressure_flag(&flag_down);
		if (!flag_down)
			break ;
	}
}

static int draw_cross16(void)
{
	unsigned int i;

	papplcd_oper->cross16(50, 50, 0xffff);
	wait_position(0);
	papplcd_oper->cross16(50, 50, 0x0);
	
	papplcd_oper->cross16(p_params->xres - 50, 50, 0xffff);
	wait_position(1);
	papplcd_oper->cross16(p_params->xres - 50, 50, 0x0);
	
	papplcd_oper->cross16(p_params->xres - 50, p_params->yres - 50, 0xffff);
	wait_position(2);
	papplcd_oper->cross16(p_params->xres - 50, p_params->yres - 50, 0x0);
	
	papplcd_oper->cross16(50, p_params->yres - 50, 0xffff);
	wait_position(3);
	papplcd_oper->cross16(50, p_params->yres - 50, 0x0);
	
	papplcd_oper->cross16(p_params->xres / 2, p_params->yres / 2, 0xffff);
	wait_position(4);
	papplcd_oper->cross16(p_params->xres / 2, p_params->yres / 2, 0x0);

	return 0;
}

void draw(unsigned int x_position, unsigned int y_position)
{
	int lcd_x, lcd_y;

	get_lcd_from_ts((int)x_position, (int)y_position, &lcd_x, &lcd_y);
	printf("lcd_x = %d, lcd_y = %d\n\r", lcd_x, lcd_y);
	papplcd_oper->dot16(lcd_x, lcd_y, 0xffff);
}

unsigned char whether_finish(void)
{
	return unfinish_lock;
}

int ts_test(void)
{
	touchscreen_init();
	tc_adc_interrupt_init();
	draw_init();
	draw_cross16();
	unfinish_lock = calibrate_xy_position(&x_position, &y_position);

	return 0;
}



