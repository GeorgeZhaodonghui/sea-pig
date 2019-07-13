#include "lcd.h"
#include "lcd_device.h"
#include "lcd_controller.h"
#include "../string_utils.h"

#define	DRIVERNAME			"s3c2440_lcd"
#define	DEVICENAME				"lcd_4_3"

plcd_device p_match_device;
plcd_controller p_match_driver;

static void lcd_register(void)
{
	lcd_device_add();
	lcd_controller_add();
}

static int select_lcd(void)
{
	p_match_device = select_lcd_device(DEVICENAME);
	if (p_match_device == NULL)
		return -1;
	p_match_driver = select_lcd_controller(DRIVERNAME);
	if (p_match_driver == NULL)
		return -1;

	return 0;
}

static int lcd_match(void)
{
	lcd_controller_init(p_match_device);

	return 0;
}

void lcd_parameters_deliver(p_app_params p_params)
{
	if (p_match_device) {
		p_params->address	= p_match_device->address;
		p_params->bpp		= p_match_device->bpp;
		p_params->xres		= p_match_device->xres;
		p_params->yres		= p_match_device->yres;
	}
}

void lcd_palette(void)
{
	lcd_controller_palette();
}

void lcd_enable(void)
{
	lcd_controller_enable();
}

void lcd_disable(void)
{
	lcd_controller_disable();
}

int lcd_init(void)
{
	int ret;

	lcd_register();
	ret = select_lcd();
	if (ret == 0)
		lcd_match();

	return 0;
}


