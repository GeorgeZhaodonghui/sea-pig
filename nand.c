#include "nand.h"
#include "s3c2440_soc.h"
#include "my_printf.h"
#include "string_utils.h"

static void nand_menu(void)
{
	printf("---------------------------------------------------------\n\r");
	printf("<i> read the id of nand flash\n\r");
	printf("<r> read the page of nand flash\n\r");
	printf("<e> earse the page of nand flash\n\r");
	printf("<w> write the page of nand flash\n\r");
	printf("Please enter the option: ");
}

static void wait_nand_ready(void)
{
	while (!(NFSTAT & 0x01));
}

void nand_init(void)
{
	/* nand flash configuration register
	  * Tacls = (Tcls/Tals) - Twp = (12/12) - 12 = 0ns
	  * Twrph0 = Twp = 12ns
	  * Twrph1 = (Tclh/Talh) = 5ns
	  */
	NFCONF &= ~((0x3 << 12) | (0x7 << 8) | (0x7 << 4));
	NFCONF |= (0x0 << 12) | (0x1 << 8) | (0x0 << 4);
	/* control register */
	NFCONT |= (0x1 << 4) | (0x1 << 1) | (0x1 << 0);
}

static void nand_chip_select(void)
{
	NFCONT &= ~(0x1 << 1);
}

static void nand_chip_deselect(void)
{
	NFCONT |= (0x1 << 1);
}

static void nand_command(unsigned char command)
{
	NFCMD = command;
	wait_nand_ready();
}

static void nand_address(unsigned char address)
{
	NFADDR	= address;
	wait_nand_ready();
}

static unsigned char nand_data(void)
{
	return NFDATA;
}

static void nand_w_data(unsigned char val)
{
	NFDATA = val;
}

static void nand_read_id(void)
{
	unsigned int block_size;
	unsigned int page_size;
	unsigned char buf[5];
	int i;

	printf("Device id: ");
	nand_chip_select();
	
	nand_command(0x90);
	nand_address(0x00);
	for (i = 0; i < 5; i++) {
		buf[i] = nand_data();
		printf("0x%x ", buf[i]);
	}
	
	nand_chip_deselect();
	printf("\n\r");

	block_size = (buf[3] >> 4) & 0x3;
	printf("block size = %dKB\n\r", (64 << block_size));
	page_size = buf[3] & 0x3;
	printf("page size = %dKB\n\r", (1 << page_size));
}

int nand_read_page(unsigned int address, unsigned int len, unsigned char *content)
{
	unsigned int i, position;
	unsigned int column, row;
	column	= (address & (2048 - 1));
	row		= (address / 2048);
	i		= address;
	position	= address + len;

	nand_chip_select();

	while (i < position) {
		nand_command(0x00);
		nand_address((column) & 0xff);
		nand_address((column >> 8) & 0x0f);
		nand_address((row) & 0xff);
		nand_address((row >> 8) & 0xff);
		nand_address((row >> 16) & 0x01);
		nand_command(0x30);
		
		do {
			*content++ = nand_data();
			i++;
		} while (((i & (2048 - 1)) != 0) && (i < position));
		
		column = 0;
		row++;
	}

	nand_chip_deselect();
	return 0;
}

static void do_nand_read(void)
{
	unsigned int i, j;
	unsigned int address;
	unsigned char buf[160];

	printf("Please enter the start address: ");
	address = get_uint();
	nand_read_page(address, 160, buf);

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 16; j++) {
			printf("%02x  ", *(buf + i * 16 + j));
		}
		printf("; ");
		for (j = 0; j < 16; j++) {
			if (*(buf + i * 16 + j) < 0x20 || *(buf + i * 16 + j) > 0x7e)
				printf(". ");
			else
				printf("%c ", *(buf + i * 16 + j));
		}
		printf("\n\r");
	}
}


static int nand_earse_page(unsigned int address, unsigned int len)
{
	unsigned int i;
	unsigned int row;
	unsigned int position;

	if (address & 0x1ffff)
		return -1;
	if (len & 0x1ffff)
		return -1;

	/* disable the soft lock */
	NFCONT &= ~(1 << 12);
	nand_chip_select();

	i = address;
	row = address / 2048;
	position = address + len;

	while (1) {
		nand_command(0x60);
		nand_address((row) & 0xff);
		nand_address((row >> 8) & 0xff);
		nand_address((row >> 16) & 0x01);
		nand_command(0xd0);

		i += (1024 * 128);
		row++;
		if (i >= position)
			break;
	}

	nand_chip_deselect();
	/* enable the soft lock */
	NFCONT |= (1 << 12);
	return 0;
}

static void do_nand_earse(void)
{
	unsigned int address;
	int ret;

	printf("Please enter the start address: ");
	address = get_uint();
	printf("earsing...\n\r");
	ret = nand_earse_page(address, (128 * 1024));
	if (ret < 0)
		printf("error, address or len is illegal\n\r");
}

static void nand_write_page(unsigned int address, unsigned char *str)
{
	unsigned int i, j, position;
	unsigned int column, row;
	
	column	= (address & (2048 - 1));
	row		= (address / 2048);
	i		= address;
	j		= 0;
	position = address + strlen(str) + 1;

	/* disable the soft lock */
	NFCONT &= ~(1 << 12);
	nand_chip_select();

	while (i < position) {
		nand_command(0x80);
		nand_address((column) & 0xff);
		nand_address((column >> 8) & 0x0f);
		nand_address((row) & 0xff);
		nand_address((row >> 8) & 0xff);
		nand_address((row >> 16) & 0x01);

		do {
			nand_w_data(str[j]);
			i++;
			j++;
		} while (((i & (2048 - 1)) != 0) && (i < position));
		
		column = 0;
		row++;
		nand_command(0x10);
	}

	nand_chip_deselect();
	/* enable the soft lock */
	NFCONT |= (1 << 12);
}

static void do_nand_write(void)
{
	unsigned int address;
	unsigned char str[100];

	printf("Please enter the start address: ");
	address = get_uint();
	printf("Enter the content of write: ");
	gets(str);

	nand_write_page(address, str);
}

static void nand_option(unsigned char option)
{
	switch (option) {
		case 'i':
		case 'I':
			nand_read_id();
			break;
		case 'r':
		case 'R':
			do_nand_read();
			break;
		case 'e':
		case 'E':
			do_nand_earse();
			break;
		case 'w':
		case 'W':
			do_nand_write();
			break;
		default:
			break;
	}

}

int nand_main(void)
{
	unsigned char option;

	nand_init();
	while (1) {
		nand_menu();
		option = getchar();
		printf("%c\n\r", option);
		nand_option(option);
	}

	return 0;
}

