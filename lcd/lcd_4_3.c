#include "lcd_device.h"

#define FRAMEBUFFER_BASE		0x33c00000

static lcd_device lcd_4_3 = {
	.name = "lcd_4_3",
	.polarity = {
		.vclk		= normal,
		.hsync	= inverted,
		.vsync	= inverted,
		.vd		= normal,
		.vden	= normal,
		.pwren	= normal,
	},
	.period = {
		.thf	= 2,
		.thp	= 41,
		.thb	= 2,
		.tvf	= 2,
		.tvp	= 10,
		.tvb	= 2,
		.vclk	= 9,
	},
	.xres = 480,
	.yres = 272,
	.bpp  = 16,
	.address = FRAMEBUFFER_BASE,
};

int lcd_4_3_add(void)
{
	int ret;
	ret = register_lcd(&lcd_4_3);
	return ret;
}


