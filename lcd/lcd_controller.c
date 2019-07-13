#include "lcd_controller.h"
#include "lcd_device.h"
#include "../string_utils.h"

#define	LCD_DRIVER_NUM		10

static plcd_controller p_lcd_driver[LCD_DRIVER_NUM];
plcd_controller p_selectlcd_driver;

int register_lcd_controller(plcd_controller plcd_driver)
{
	int i;

	for (i = 0; i < LCD_DRIVER_NUM; i++) {
		if (!p_lcd_driver[i]) {
			p_lcd_driver[i] = plcd_driver;
			return i;
		}
	}

	return -1;
}

plcd_controller select_lcd_controller(const unsigned char *name)
{
	int i;

	for (i = 0; i < LCD_DRIVER_NUM; i++) {
		if (p_lcd_driver[i] && !strcmp(name, p_lcd_driver[i]->name)) {
			p_selectlcd_driver = p_lcd_driver[i];
			return p_selectlcd_driver;
		}
	}

	return NULL;
}

void lcd_controller_init(plcd_device p_select_device)
{
	p_selectlcd_driver->lcd_init(p_select_device);
}

void lcd_controller_palette(void)
{
	p_selectlcd_driver->lcd_palette();
}

void lcd_controller_enable(void)
{
	p_selectlcd_driver->lcd_enable();
}

void lcd_controller_disable(void)
{
	p_selectlcd_driver->lcd_disable();
}

void lcd_controller_add(void)
{
	lcd_s3c2440_add();
}


