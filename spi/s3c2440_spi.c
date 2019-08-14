#include "s3c2440_spi.h"
#include "../s3c2440_soc.h"

static void gpio_spi_init(void)
{
	GPGCON &= ~((0x3 << 14) | (0x3 << 12) | (0x3 << 10) | (0x3 << 8) | (0x3 << 4));
	GPGCON |= (0x3 << 14) | (0x3 << 12) | (0x3 << 10) | (0x1 << 8) | (0x1 << 4);
	GPGDAT |= (0x1 << 2);

	GPFCON &= ~(0x3 << 2);
	GPFCON |= (0x1 << 2);
	GPFDAT |= (0x1 << 1);

	return ;
}

static void spi_controller_init(void)
{
	SPPRE1 = 2;
	SPCON1 = (0x0 << 5) | (0x1 << 4) | (0x1 << 3) | (0x0 << 2) | (0x0 << 1) | (0x0 << 0);

	return ;
}

static void spi_status_ready(void)
{
	while (!(SPSTA1 & 0x1));

	return ;
}

void gpio_spi_transfer_tx(unsigned char data)
{
	spi_status_ready();
	SPTDAT1 = data;

	return ;
}

unsigned char gpio_spi_transfer_rx(void)
{
	unsigned char val;

	SPTDAT1 = 0xff;
	spi_status_ready();
	val = SPRDAT1;

	return val;
}

void spi_init(void)
{
	gpio_spi_init();
	spi_controller_init();

	return ;
}


