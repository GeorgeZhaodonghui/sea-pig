#include "../s3c2440_soc.h"
#include "lcd_controller.h"

#define	PALETTE_ADDRESS			0x4d000400

static int s3c2440_lcd_pin(void)
{
	GPBCON &= ~(0x3 << 0);
	GPBCON |= (0x2 << 0);
	GPCCON = 0xaaaaaaaa;
	GPDCON = 0xaaaaaaaa;
	GPGCON |= (0x3 << 8);

	return 0;
}

static int s3c2440_lcd_init(plcd_device plcd_dev)
{
	unsigned int clkval;
	unsigned int bppmode;
	unsigned int bppsave;
	unsigned int format;
	unsigned int address;

	/* gpio init */
	s3c2440_lcd_pin();

	/* lcd control register1 */
	clkval = 100 / (plcd_dev->period.vclk * 2) - 1 + 0.5;
	if (plcd_dev->bpp == 8)
		bppmode = 0xb;
	else if (plcd_dev->bpp == 16)
		bppmode = 0xc;
	else
		bppmode = 0xd;
	LCDCON1 &= ~((0x3ff << 8) | (0x3 << 5) | (0xf << 1));
	LCDCON1 |= (clkval << 8) | (3 << 5) | (bppmode << 1);

	/* lcd control register2
	  * VSPW+1 = tvp; VBPD+1 = tvb; LINEVAL+1 = tvd; VFPD+1 = tvf;
	  */
	LCDCON2 &= ~((0xff << 24) | (0x3ff << 14) | (0xff << 6) | (0x3f << 0));
	LCDCON2 |= ((plcd_dev->period.tvb - 1) << 24) | ((plcd_dev->yres - 1) << 14) |\
				((plcd_dev->period.tvf - 1) << 6) | ((plcd_dev->period.tvp - 1) << 0);

	/* lcd control register3
	  * HBPD+1 = thb; HOZVAL+1 = thd; HFPD+1 = thf;
	  */
	LCDCON3 &= ~((0x7f << 19) | (0x7ff << 8) | (0xff << 0));
	LCDCON3 |= ((plcd_dev->period.thb - 1) << 19) |\
				((plcd_dev->xres - 1) << 8) | ((plcd_dev->period.thf - 1) << 0);

	/* lcd control register4
	  * HSPW+1 = thp;
	  */
	LCDCON4 &= ~(0xff << 0);
	LCDCON4 |= ((plcd_dev->period.thp - 1) << 0);

	/* lcd control register5 */
	if (plcd_dev->bpp == 8)
		format = 0x2;
	else if (plcd_dev->bpp == 16)
		format = 0x1;
	else
		format = 0x0;
	LCDCON5 =	(plcd_dev->polarity.vclk << 10) |\
				(plcd_dev->polarity.hsync << 9) |\
				(plcd_dev->polarity.vsync << 8) |\
				(plcd_dev->polarity.vd << 7) |\
				(plcd_dev->polarity.vden << 6) |\
				(plcd_dev->polarity.pwren << 5) |\
				(0x1 << 11) | (format << 0);

	/* frame buffer start address 1 register */
	address = (plcd_dev->address) & ~(1 << 31);
	LCDSADDR1 = address >> 1;

	/* frame buffer start address 2 register */
	if (plcd_dev->bpp == 8)
		bppsave = 1;
	else if (plcd_dev->bpp == 16)
		bppsave = 2;
	else
		bppsave = 4;
	address = plcd_dev->address + plcd_dev->xres * plcd_dev->yres * bppsave;
	address = address >> 1;
	address = address & 0x1fffff;
	LCDSADDR2 = address;

	return 0;
}

static int s3c2440_lcd_palette(void)
{
	unsigned int *address = (unsigned int *)PALETTE_ADDRESS;
	unsigned int i;
	unsigned int status;

	status = (LCDCON1 & 0x1);
	if (status)
		LCDCON1 &= ~(0x1 << 0);

	for (i = 0; i < 256; i++) {
		*address = i * 256;
		address++;
	}

	if (status)
		LCDCON1 |= (0x1 << 0);

	return 0;
}

static int s3c2440_lcd_enable(void)
{
	/* enable lcd control signal */
	LCDCON1 |= (0x1 << 0);
	/* lcd power enable */
	LCDCON5 |= (0x1 << 3);
	/* backlight */
	GPBDAT |= (0x1 << 0);

	return 0;
}

static void s3c2440_lcd_disable(void)
{
	/* disable lcd control signal */
	LCDCON1 &= ~(0x1 << 0);
	/* lcd power disable */
	LCDCON5 &= ~(0x1 << 3);
	/* backlight */
	GPBDAT &= ~(0x1 << 0);
}

static lcd_controller s3c2440_lcd = {
	.name		= "s3c2440_lcd",
	.lcd_init		= s3c2440_lcd_init,
	.lcd_palette	= s3c2440_lcd_palette,
	.lcd_enable	= s3c2440_lcd_enable,
	.lcd_disable	= s3c2440_lcd_disable,
};

int lcd_s3c2440_add(void)
{
	int ret;
	ret = register_lcd_controller(&s3c2440_lcd);
	return ret;
}

