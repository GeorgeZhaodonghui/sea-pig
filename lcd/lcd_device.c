#include "lcd_device.h"
#include "../string_utils.h"

#define	LCD_NUM		10

static plcd_device p_lcd_device[LCD_NUM];
plcd_device p_selectlcd_device;

int register_lcd(plcd_device plcd_dev)
{
	int i;

	for (i = 0; i < LCD_NUM; i++) {
		if (!p_lcd_device[i]) {
			p_lcd_device[i] = plcd_dev;
			return i;
		}
	}

	return -1;
}

plcd_device select_lcd_device(const unsigned char *name)
{
	int i;

	for (i = 0; i < LCD_NUM; i++) {
		if (p_lcd_device[i] && !strcmp(name, p_lcd_device[i]->name)) {
			p_selectlcd_device = p_lcd_device[i];
			return p_selectlcd_device;
		}
	}

	return NULL;
}

void lcd_device_add(void)
{
	lcd_4_3_add();
}


