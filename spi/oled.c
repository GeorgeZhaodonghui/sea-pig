#include "oled.h"
#include "s3c2440_spi.h"
#include "../s3c2440_soc.h"
#include "oled_font.h"

static void set_oled_cs(unsigned char val)
{
	if (val)
		GPFDAT |= (0x1 << 1);
	else
		GPFDAT &= ~(0x1 << 1);
}

static void set_oled_dc(unsigned char val)
{
	if (val)
		GPGDAT |= (0x1 << 4);
	else
		GPGDAT &= ~(0x1 << 4);
}

static void oled_write_command(unsigned char ins)
{
	set_oled_dc(0);
	set_oled_cs(0);

	gpio_spi_transfer_tx(ins);

	set_oled_cs(1);
	set_oled_dc(1);
}

static void oled_write_data(unsigned char data)
{
	set_oled_dc(1);
	set_oled_cs(0);

	gpio_spi_transfer_tx(data);

	set_oled_cs(1);
	set_oled_dc(1);
}

static void oled_set_addr_mode(void)
{
	oled_write_command(0x20);
	oled_write_command(0x02);
}

static void oled_set_postion(unsigned int page, unsigned int col)
{
	/* set page */
	oled_write_command(0xb0 + page);

	/* set column */
	oled_write_command(0x0f & col);
	oled_write_command(0x10 + (col >> 4));
}

static void oled_putchar(unsigned int page, unsigned int col, unsigned char chr)
{
	int i;
	unsigned char *dots;
	//const unsigned char *dots = oled_8x16_font[chr - ' '];
	// get font
	dots = (unsigned char *)oled_8x16_font[chr - ' '];

	// draw
	oled_set_postion(page, col);
	for (i = 0; i < 8; i++)
		oled_write_data(dots[i]);

	oled_set_postion(page + 1, col);
	for (i = 0; i < 8; i++)
		oled_write_data(dots[i + 8]);

	return ;
}

void oled_clear_all(void)
{
	unsigned int page;
	unsigned int col;

	for (page = 0; page < 8; page++) {
		oled_set_postion(page, 0);
		for (col = 0; col < 128; col++) {
			oled_write_data(0);
		}
	}

	return ;
}

void oled_clear_page(unsigned char page)
{
	unsigned int col;

	oled_set_postion(page, 0);
	for (col = 0; col < 128; col++) {
		oled_write_data(0);
	}

	return ;
}

void oled_init(void)
{
	oled_write_command(0xAE); /*display off*/
	oled_write_command(0x00); /*set lower column address*/
	oled_write_command(0x10); /*set higher column address*/
	oled_write_command(0x40); /*set display start line*/
	oled_write_command(0xB0); /*set page address*/
	oled_write_command(0x81); /*contract control*/
	oled_write_command(0x66); /*128*/
	oled_write_command(0xA1); /*set segment remap*/
	oled_write_command(0xA6); /*normal / reverse*/
	oled_write_command(0xA8); /*multiplex ratio*/
	oled_write_command(0x3F); /*duty = 1/64*/
	oled_write_command(0xC8); /*Com scan direction*/
	oled_write_command(0xD3); /*set display offset*/
	oled_write_command(0x00);
	oled_write_command(0xD5); /*set osc division*/
	oled_write_command(0x80);
	oled_write_command(0xD9); /*set pre-charge period*/
	oled_write_command(0x1f);
	oled_write_command(0xDA); /*set COM pins*/
	oled_write_command(0x12);
	oled_write_command(0xdb); /*set vcomh*/
	oled_write_command(0x30);
	oled_write_command(0x8d); /*set charge pump enable*/
	oled_write_command(0x14);

	oled_set_addr_mode();
	oled_clear_all();

	oled_write_command(0xAF); /*display ON*/
}

void oled_printf(unsigned int page, unsigned int col, unsigned char *str)
{
	int i = 0;

	while (str[i]) {	
		if (col >= 127) {
			col = 0;
			page += 2;
		}

		if (page >= 7)
			page = 0;

		oled_putchar(page, col, str[i]);
		col += 8;
		i++;
	}

	return ;
}

