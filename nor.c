#include "nor.h"
#include "string_utils.h"
#include "my_printf.h"
#include "uart.h"

static void nor_menu(void)
{
	printf("***************************************************\n\r");
	printf("Nor Flash operation menu\n\r");
	printf("<s> scan the information of nor flash\n\r");
	printf("<r> read nor flash\n\r");
	printf("<w> write nor flash\n\r");
	printf("<e> Erase nor flash\n\r");
	printf("<q> quit\n\r");
	printf("Pleasr enter the option above:");
}

static void nor_transmit(unsigned int addr, unsigned short data)
{
	volatile unsigned short *paddr = (volatile unsigned short *)(NOR_BASE_ADDRESS + (addr << 1));

	*paddr = data;
}

static unsigned short nor_receive(unsigned int addr)
{
	volatile unsigned short *paddr = (volatile unsigned short *)(NOR_BASE_ADDRESS + (addr << 1));

	return (*paddr);
}

static void nor_scan_cfi(void)
{
	unsigned char str[4];
	unsigned int size;
	unsigned int num_regions;
	unsigned int regions_feedback[4];
	unsigned int region, block;
	unsigned int position = 0;
	unsigned int i, j, cnt = 0;

	/* Enter cfi */
	nor_transmit(0x55, 0x98);
	
	/* Query unique ASCII string "QRY" */
	str[0] = nor_receive(0x10);
	str[1] = nor_receive(0x11);
	str[2] = nor_receive(0x12);
	str[3] = '\0';
	printf("Enter the state of %s\n\r", str);

	/* device size */
	size = (1 << nor_receive(0x27));
	printf("The device size is %d, %dM\n\r", size, (size / (1024 * 1024)));

	/* regions within device */
	num_regions = nor_receive(0x2c);
	for (i = 0; i < num_regions; i++) {
		for (j = 0; j < 4; j++) {
			regions_feedback[j] = nor_receive((0x2d + i * 4 + j));
		}
		region = (regions_feedback[0] + (regions_feedback[1] << 8)) + 1;
		block  = (regions_feedback[2] + (regions_feedback[3] << 8)) * 256;
		for (j = 0; j < region; j++) {
			printf("0x%08x  ", position);
			position += block;
			cnt++;
			if ((cnt % 5) == 0)
				printf("\n\r");
		}
	}

	/* quit cfi */
	nor_transmit(0x00, 0xf0);
}

static void nor_scan_id(void)
{
	unsigned int device_id[2];

	nor_transmit(0x555, 0xaa);
	nor_transmit(0x2aa, 0x55);
	nor_transmit(0x555, 0x90);
	device_id[0] = nor_receive(0x0);
	device_id[1] = nor_receive(0x1);
	printf("The manufacture ID: 0x%x\n\r", device_id[0]);
	printf("The Device ID: 0x%x\n\r", device_id[1]);
	nor_transmit(0x00, 0xf0);
}

static void do_nor_scan(void)
{
	nor_scan_cfi();
	nor_scan_id();
}

static void do_nor_read(void)
{
	unsigned char *paddr;
	unsigned int address;
	unsigned int i, j;
	unsigned char buf[16];

	printf("Enter the address of nor flash to read: ");
	address = get_uint();
	paddr = (unsigned char *)address;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 16; j++) {
			buf[j] = *paddr++;
			printf("%02x  ", buf[j]);
		}
		printf("; ");
		for (j = 0; j < 16; j++) {
			if (buf[j] < 0x20 || buf[j] > 0x7e)
				printf(". ");
			else
				printf("%c ", buf[j]);
		}
		printf("\n\r");
	}
}

static void wait_ready(unsigned int address)
{
	unsigned short pre;
	unsigned short val;

	pre = nor_receive(address >> 1);
	val = nor_receive(address >> 1);
	while ((pre & 0x40) != (val & 0x40)) {
		pre = val;
		val = nor_receive(address >> 1);
	}
}

static void do_nor_write(void)
{
	unsigned int address;
	char str[100];
	unsigned short val;
	unsigned int i = 0, j = 1;

	printf("Enter the address of nor flash to write: ");
	address = get_uint();
	printf("Enter the content of write: ");
	gets(str);

	void private_write(unsigned int address, unsigned short val)
	{
		nor_transmit(0x555, 0xaa);
		nor_transmit(0x2aa, 0x55);
		nor_transmit(0x555, 0xa0);
		nor_transmit((address >> 1), val);
	}

	while ((str[i] != 0) && str[j] != 0) {
		val = str[i] + (str[j] << 8);
		private_write(address, val);
		i += 2;
		j += 2;
		address += 2;
	}
	val = str[i];
	private_write(address, val);

	printf("write......\n\r");
	wait_ready(address);
	printf("written\n\r");
}

static void do_nor_erase(void)
{
	unsigned int address;

	printf("Enter the address of nor flash to erase: ");
	address = get_uint();
	nor_transmit(0x555, 0xaa);
	nor_transmit(0x2aa, 0x55);
	nor_transmit(0x555, 0x80);
	nor_transmit(0x555, 0xaa);
	nor_transmit(0x2aa, 0x55);
	nor_transmit((address >> 1), 0x30);

	printf("erasing......\n\r");
	wait_ready(address);
	printf("erased\n\r");
}

static int nor_option(unsigned char option)
{
	switch (option) {
		case 's':
		case 'S':
			do_nor_scan();
			break;

		case 'r':
		case 'R':
			do_nor_read();
			break;
			
		case 'w':
		case 'W':
			do_nor_write();
			break;
			
		case 'e':
		case 'E':
			do_nor_erase();
			break;
			
		case 'q':
		case 'Q':
			return 0;
			break;
			
		default:
			break;
	}

	return 0;
}

void nor_main(void)
{
	unsigned char option;

	while (1) {
		nor_menu();
		option = getchar();
		printf("%c\n\r", option);
		nor_option(option);
	}
}

