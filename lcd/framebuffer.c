#include "framebuffer.h"
#include "lcd.h"
#include "font.h"

static p_app_params p_params;

static void lcd_dot24(int x, int y, unsigned int colour)
{
	unsigned int position;
	unsigned int *draw;

	if (x >= p_params->xres)
		return ;
	if (y >= p_params->yres)
		return ;
	
	position = p_params->address + (y * p_params->xres + x) * 4;
	draw = (unsigned int *)position;
	*draw = colour & 0xffffff;
}

static void lcd_circle24(int x, int y, int r, unsigned int colour)  
{	
	int a, b, num;
	a = 0;
	b = r;
	while(22 * b * b >= r * r) {
		lcd_dot24(x + a, y - b, colour);
		lcd_dot24(x - a, y - b, colour);
		lcd_dot24(x - a, y + b, colour);
		lcd_dot24(x + a, y + b, colour);

		lcd_dot24(x + b, y + a, colour);
		lcd_dot24(x + b, y - a, colour);
		lcd_dot24(x - b, y - a, colour);
		lcd_dot24(x - b, y + a, colour);

		a++;
		num = (a * a + b * b) - r*r;
		if(num > 0) {	
			b--;  
			a--;  
		}
	}
}

static void lcd_dot16(int x, int y, unsigned short colour)
{
	unsigned int position;
	unsigned short *draw;

	if (x >= p_params->xres)
		return ;
	if (y >= p_params->yres)
		return ;
	
	position = p_params->address + (y * p_params->xres + x) * 2;
	draw = (unsigned short *)position;
	*draw = colour;
}

static void lcd_circle16(int x, int y, int r, unsigned short colour)  
{	
	int a, b, num;
	a = 0;
	b = r;
	while(22 * b * b >= r * r) {
		lcd_dot16(x + a, y - b, colour);
		lcd_dot16(x - a, y - b, colour);
		lcd_dot16(x - a, y + b, colour);
		lcd_dot16(x + a, y + b, colour);

		lcd_dot16(x + b, y + a, colour);
		lcd_dot16(x + b, y - a, colour);
		lcd_dot16(x - b, y - a, colour);
		lcd_dot16(x - b, y + a, colour);

		a++;
		num = (a * a + b * b) - r*r;
		if(num > 0) {
			b--;
			a--;
		}
	}
}

static void lcd_dot8(int x, int y, unsigned char colour)
{
	unsigned int position;
	unsigned char *draw;

	if (x >= p_params->xres)
		return ;
	if (y >= p_params->yres)
		return ;
	
	position = p_params->address + (y * p_params->xres + x) * 1;
	draw = (unsigned char *)position;
	*draw = colour;
}

static void lcd_draw16_cross(int x, int y, unsigned short colour)
{
	unsigned int i;
	
	for (i = 0; i < 21; i++)
		lcd_dot16(x - 10 + i, y, colour);
	for (i = 0; i < 21; i++)
		lcd_dot16(x, y - 10 + i, colour);
}

static lcd_operations lcd_oper = {
	.dot8	= lcd_dot8,
	.circle16	= lcd_circle16,
	.dot16	= lcd_dot16,
	.cross16	= lcd_draw16_cross,
	.circle24	= lcd_circle24,
	.dot24	= lcd_dot24,
};

static void frambuffer_init(void)
{
	unsigned int i, j;

	for (i = 0; i < p_params->yres; i++) {
		for (j = 0; j < p_params->xres; j++) {
			if (p_params->bpp == 16)
				lcd_dot16(j, i, 0x0);
			else if (p_params->bpp == 24)
				lcd_dot24(j, i, 0x0);
			else if (p_params->bpp == 8)
				lcd_dot8(j, i, 0x0);
		}
	}
}

static plcd_operations register_lcd_operstions(plcd_operations plcd_oper)
{
	return plcd_oper;
}

plcd_operations lcd_hardware(void)
{
	plcd_operations papplcd_oper;

	lcd_init();
	lcd_palette();
	lcd_enable();
	lcd_parameters_deliver(p_params);
	frambuffer_init();
	papplcd_oper = register_lcd_operstions(&lcd_oper);

	return papplcd_oper;
}

