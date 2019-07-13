#ifndef _LCD_CONTROLLER
#define _LCD_CONTROLLER

#include "lcd_device.h"

typedef struct lcd_controller {
	unsigned char *name;
	int (*lcd_init)(plcd_device plcd_dev);
	int (*lcd_palette)(void);
	int (*lcd_enable)(void);
	void (*lcd_disable)(void);
}lcd_controller, *plcd_controller;

int register_lcd_controller(plcd_controller plcd_driver);
plcd_controller select_lcd_controller(const unsigned char *name);
void lcd_controller_add(void);
void lcd_controller_init(plcd_device p_select_device);
void lcd_controller_palette(void);
void lcd_controller_enable(void);
void lcd_controller_disable(void);

#endif

