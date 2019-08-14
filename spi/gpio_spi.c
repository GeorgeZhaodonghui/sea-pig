#include "gpio_spi.h"
#include "../s3c2440_soc.h"

static void gpio_spi_init(void)
{
	GPGCON &= ~((0x3 << 14) | (0x3 << 12) | (0x3 << 10) | (0x3 << 8) | (0x3 << 4));
	GPGCON |= (0x1 << 14) | (0x1 << 12) | (0x1 << 8) | (0x1 << 4);
	GPGDAT |= (0x1 << 2);

	GPFCON &= ~(0x3 << 2);
	GPFCON |= (0x1 << 2);
	GPFDAT |= (0x1 << 1);
}

static void set_spi_clk(unsigned char val)
{
	if (val)
		GPGDAT |= (0x1 << 7);
	else
		GPGDAT &= ~(0x1 << 7);
}

static void set_spi_mosi(unsigned char val)
{
	if (val)
		GPGDAT |= (0x1 << 6);
	else
		GPGDAT &= ~(0x1 << 6);
}

static unsigned char set_spi_miso(void)
{
	if (GPGDAT & (0x1 << 5))
		return 1;
	else
		return 0;
}

void gpio_spi_transfer_tx(unsigned char data)
{
	int i;

	for (i = 0; i < 8; i++) {
		set_spi_clk(0);
		set_spi_mosi(data & 0x80);
		set_spi_clk(1);
		data <<= 1;
	}

	return ;
}

unsigned char gpio_spi_transfer_rx(void)
{
	int i;
	unsigned char val;

	for (i = 0; i < 8; i++) {
		val <<= 1;
		set_spi_clk(0);
		val |= set_spi_miso();
		set_spi_clk(1);
	}

	return val;
}

void spi_init(void)
{
	gpio_spi_init();

	return ;
}

