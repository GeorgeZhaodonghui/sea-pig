#include "i2c_controller.h"
#include "at24c02.h"
#include "../string_utils.h"
#include "../my_printf.h"
#include "../leds.h"

static void at24c02_menu(void)
{
	printf("***************************************************\n\r");
	printf("At24c02 operation menu\n\r");
	printf("<r> read nor flash\n\r");
	printf("<w> write nor flash\n\r");
	printf("<q> quit\n\r");
	printf("Pleasr enter the option above:");
}

static void do_at24c02_write(void)
{
	unsigned int address;
	char str[100];
	unsigned short val;
	unsigned int i = 0, j = 1;

	printf("Enter the address of at24c02 to write: ");
	address = get_uint();
	printf("Enter the content of write: ");
	gets(str);

	at24c02_write(address, str, strlen(str));

	printf("written\n\r");
}

static void do_at24c02_read(void)
{
	unsigned char *paddr;
	unsigned int address;
	unsigned int i, j;
	unsigned char buf[17];

	printf("Enter the address of at24c02 to read: ");
	address = get_uint();

	for (i = 0; i < 4; i++) {
		at24c02_read(address, buf, 17);
		for (j = 0; j < 16; j++) {
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
		address += 16;
	}
}

static int at24c02_option(unsigned char option)
{
	switch (option) {
		case 'r':
		case 'R':
			do_at24c02_read();
			break;
			
		case 'w':
		case 'W':
			do_at24c02_write();
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

void i2c_test(void)
{
	unsigned char option;

	i2c_init();

	while (1) {
		at24c02_menu();
		option = getchar();
		printf("%c\n\r", option);
		at24c02_option(option);
	}
}

