#include "spi_flash.h"
#include "gpio_spi.h"
#include "../s3c2440_soc.h"

void gpio_spi_transfer_tx(unsigned char data);
unsigned char gpio_spi_transfer_rx(void);

static void spi_flash_cs(unsigned char val)
{
	if (val)
		GPGDAT |= (0x1 << 2);
	else
		GPGDAT &= ~(0x1 << 2);
}

static void spi_flash_send_address(unsigned int address)
{
	gpio_spi_transfer_tx(address >> 16);
	gpio_spi_transfer_tx(address >> 8);
	gpio_spi_transfer_tx(address & 0xff);
}

void spi_flash_read_id(unsigned char *pid, unsigned char *vid)
{
	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x90);
	spi_flash_send_address(0x0);

	*pid = gpio_spi_transfer_rx();
	*vid = gpio_spi_transfer_rx();
	spi_flash_cs(1);
}


