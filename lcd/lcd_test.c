//#include "framebuffer.h"
#include "font.h"

void lcd_test(void)
{
	lcd_font_init();
	//lcd_string16(0, 0, 0xf800, "hello world\n\rI am George");
	lcd_string24(200, 100, 0x00ff00, "hello world\n\rI am George");
}

/*
void lcd_test(void)
{
	int i;
	p_app_params p_params;
	
	lcd_hardware();
	lcd_parameters_deliver(p_params);

	if (p_params->bpp == 16) {
		for(i = 0; i < 200; i++) {
			papplcd_oper->dot16(200, i, 0xf800);
			papplcd_oper->dot16(100, i, 0x07e0);
			papplcd_oper->dot16(i, 200, 0x001f);
		}
		papplcd_oper->circle16(200, 100, 50, 0x07e0);
	} else if (p_params->bpp == 24) {
		for(i = 0; i < 400; i++) {
			papplcd_oper->dot24(400, i, 0xff0000);
			papplcd_oper->dot24(300, i, 0x00ff00);
			papplcd_oper->dot24(i, 200, 0x0000ff);
		}
		papplcd_oper->circle24(300, 100, 100, 0xff0000);
	}
}


void lcd_test(void)
{
	unsigned short *p16;
	unsigned int *p24;
	unsigned int xres;
	unsigned int yres;

	p_app_params p_params;

	lcd_init();
	lcd_enable();
	lcd_parameters_deliver(p_params);

	if (p_params->bpp == 16) {

		p16 = (unsigned short *)p_params->address;
		for (yres = 0; yres < p_params->yres; yres++)
			for(xres = 0; xres < p_params->xres; xres++)
				*p16++ = 0xf800;

		p16 = (unsigned short *)p_params->address;
		for (yres = 0; yres < p_params->yres; yres++)
			for(xres = 0; xres < p_params->xres; xres++)
				*p16++ = 0x07e0;

		p16 = (unsigned short *)p_params->address;
		for (yres = 0; yres < p_params->yres; yres++)
			for(xres = 0; xres < p_params->xres; xres++)
				*p16++ = 0x001f;
	} else if (p_params->bpp == 24) {
		p24 = (unsigned int *)p_params->address;
		for (yres = 0; yres < p_params->yres; yres++)
			for(xres = 0; xres < p_params->xres; xres++)
				*p24++ = 0xff0000;

		p24 = (unsigned int *)p_params->address;
		for (yres = 0; yres < p_params->yres; yres++)
			for(xres = 0; xres < p_params->xres; xres++)
				*p24++ = 0x00ff00;

		p24 = (unsigned int *)p_params->address;
		for (yres = 0; yres < p_params->yres; yres++)
			for(xres = 0; xres < p_params->xres; xres++)
				*p24++ = 0x0000ff;
	}
}
*/

