#include "framebuffer.h"
#include "lcd.h"
#include "font.h"

extern unsigned char fontdata_8x16[];

static plcd_operations papplcd_oper;
static p_app_params p_params;

int lcd_font_init(void)
{
	papplcd_oper = lcd_hardware();
	lcd_parameters_deliver(p_params);

	return 0;
}

int lcd_char16(int x, int y, unsigned short colour, unsigned char c)
{
	int i, j;
	unsigned char *pfont;

	pfont = fontdata_8x16 + c * 16;
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 8; j++) {
			if ((*(pfont + i) >> (7 - j)) & 0x1)
				papplcd_oper->dot16(x + j, y + i, colour);
			else
				papplcd_oper->dot16(x + j, y + i, 0x0);
		}
	}

	return 0;
}

int lcd_string16(int x, int y, unsigned short colour, unsigned char *str)
{
	int i = 0;

	while (str[i]) {
		
		if (str[i] == '\r')
			x = 0;
		else if (str[i] == '\n')
			y += 16;
		else {
			lcd_char16(x , y, colour, str[i]);
			x += 8;
		}
		i++;
		
		if (x >= (p_params->xres - 8)) {
			x = 0;
			y += 16;
		}

		if (y >= (p_params->yres - 8)) {
			y = 0;
			x = 0;
		}
	}
}

int lcd_char24(int x, int y, unsigned int colour, unsigned char c)
{
	int i, j;
	unsigned char *pfont;

	pfont = fontdata_8x16 + c * 16;
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 8; j++) {
			if ((*(pfont + i) >> (7 - j)) & 0x1)
				papplcd_oper->dot24(x + j, y + i, colour);
			else
				papplcd_oper->dot24(x + j, y + i, 0x0);
		}
	}

	return 0;
}

int lcd_string24(int x, int y, unsigned int colour, unsigned char *str)
{
	int i = 0;

	while (str[i]) {
		
		if (str[i] == '\r')
			x = 0;
		else if (str[i] == '\n')
			y += 16;
		else {
			lcd_char24(x , y, colour, str[i]);
			x += 8;
		}
		i++;
		
		if (x >= (p_params->xres - 8)) {
			x = 0;
			y += 16;
		}

		if (y >= (p_params->yres - 8)) {
			y = 0;
			x = 0;
		}
	}
}

int lcd_char8(int x, int y, unsigned char colour, unsigned char c)
{
	int i, j;
	unsigned char *pfont;

	pfont = fontdata_8x16 + c * 16;
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 8; j++) {
			if ((*(pfont + i) >> (7 - j)) & 0x1)
				papplcd_oper->dot8(x + j, y + i, colour);
			else
				papplcd_oper->dot8(x + j, y + i, 0x0);
		}
	}

	return 0;
}

