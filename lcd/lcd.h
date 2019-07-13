#ifndef _LCD_H
#define _LCD_H

typedef struct app_params {
	unsigned int address;
	unsigned int xres;
	unsigned int yres;
	unsigned int bpp;
}app_params, *p_app_params;

int lcd_init(void);
void lcd_palette(void);
void lcd_enable(void);
void lcd_disable(void);
void lcd_parameters_deliver(p_app_params p_params);

#endif


