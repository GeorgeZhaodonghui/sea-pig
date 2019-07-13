#ifndef _LCD_DEVICE
#define _LCD_DEVICE

typedef enum polarity_switch{
	normal	= 0,
	inverted	= 1,
}polarity_switch;

typedef struct pin_polarity {
	polarity_switch vclk;
	polarity_switch hsync;
	polarity_switch vsync;
	polarity_switch vd;
	polarity_switch vden;
	polarity_switch pwren;
}pin_polarity;

typedef struct lcd_period {
	/* horizontal */
	unsigned int thf;
	unsigned int thp;
	unsigned int thb;
	/* vertical */
	unsigned int tvf;
	unsigned int tvp;
	unsigned int tvb;
	/* clock cycle */
	unsigned int vclk;
}lcd_period;

typedef struct lcd_device {
	unsigned char *name;
	/* polarity */
	pin_polarity polarity;
	/* period */
	lcd_period period;
	/* resolution, bpp */
	unsigned int xres;
	unsigned int yres;
	unsigned int bpp;
	/* framebuffer base address */
	unsigned int address;
}lcd_device, *plcd_device;

int register_lcd(plcd_device plcd_dev);
plcd_device select_lcd_device(const unsigned char *name);
void lcd_device_add(void);

#endif

