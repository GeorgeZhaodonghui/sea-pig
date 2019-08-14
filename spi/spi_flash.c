#include "spi_flash.h"
#include "s3c2440_spi.h"
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

	return ;
}

void spi_flash_read_id(unsigned char *pid, unsigned char *vid)
{
	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x90);
	spi_flash_send_address(0x0);

	*pid = gpio_spi_transfer_rx();
	*vid = gpio_spi_transfer_rx();
	spi_flash_cs(1);

	return ;
}

static void spi_flash_write_enable(unsigned char val)
{
	if (val) {
		spi_flash_cs(0);
		gpio_spi_transfer_tx(0x06);
		spi_flash_cs(1);
	} else {
		spi_flash_cs(0);
		gpio_spi_transfer_tx(0x04);
		spi_flash_cs(1);
	}

	return ;
}

static unsigned char spi_flash_read_status_reg1(void)
{
	unsigned char status;

	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x05);
	status = gpio_spi_transfer_rx();
	spi_flash_cs(1);

	return status;
}

static unsigned char spi_flash_read_status_reg2(void)
{
	unsigned char status;

	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x35);
	status = gpio_spi_transfer_rx();
	spi_flash_cs(1);

	return status;
}

static void spi_flash_waiting_busy(void)
{
	while (spi_flash_read_status_reg1() & 0x1);
}

static void spi_flash_write_status_reg(unsigned char status1, unsigned char status2)
{
	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x01);
	gpio_spi_transfer_tx(status1);
	gpio_spi_transfer_tx(status2);
	spi_flash_cs(1);
	
	spi_flash_waiting_busy();

	return ;
}

static void spi_flash_clear_status_protect(void)
{
	unsigned char status1;
	unsigned char status2;

	status1 = spi_flash_read_status_reg1();
	status2 = spi_flash_read_status_reg2();

	status1 &= ~(0x1 << 7);
	status2 &= ~(0x1 << 0);
	spi_flash_write_status_reg(status1, status2);

	return ;
}

static void spi_flash_clear_data_protect(void)
{
	unsigned char status1;
	unsigned char status2;

	status1 = spi_flash_read_status_reg1();
	status2 = spi_flash_read_status_reg2();

	status1 &= ~(0x7 << 2);
	status2 &= ~(0x1 << 6);
	spi_flash_write_status_reg(status1, status2);

	return ;
}

void spi_flash_sector_erase(unsigned int address)
{
	spi_flash_write_enable(1);

	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x20);
	spi_flash_send_address(address);
	spi_flash_cs(1);

	spi_flash_write_enable(0);
	spi_flash_waiting_busy();

	return ;
}

void spi_flash_page_program(unsigned int address, unsigned char *buf, unsigned int len)
{
	int i;

	spi_flash_write_enable(1);

	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x02);
	spi_flash_send_address(address);
	for (i = 0; i < len; i++)
		gpio_spi_transfer_tx(buf[i]);
	
	spi_flash_cs(1);
	spi_flash_write_enable(0);
	spi_flash_waiting_busy();

	return ;
}

void spi_flash_read_data(unsigned int address, unsigned char *buf, unsigned int len)
{
	int i;

	spi_flash_cs(0);
	gpio_spi_transfer_tx(0x03);
	spi_flash_send_address(address);
	for (i = 0; i < len; i++)
		buf[i] = gpio_spi_transfer_rx();

	spi_flash_cs(1);

	return ;
}

void spi_flash_init(void)
{
	spi_flash_clear_status_protect();
	spi_flash_clear_data_protect();

	return ;
}


