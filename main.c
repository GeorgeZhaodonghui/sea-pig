#include "uart.h"
#include "my_printf.h"
#include "exception.h"
#include "init.h"
#include "leds.h"
#include "key.h"
#include "timer.h"
#include "nor.h"
#include "lcd/font.h"
#include "lcd/lcd.h"
#include "ts/ts_test.h"
#include "spi/gpio_spi.h"
#include "spi/oled.h"
#include "sensor/delay.h"

char	g_char = 'A';
char g_char2 = 'a';
const char g_const = 'B';
int	g_a = 0;
int	g_b;

int main(void)
{
	unsigned int adc;
	double voltage;
	unsigned int m0, m1;
	unsigned int n0, n1;
	unsigned char pid;
	unsigned char vid;
	//char str1[100];
	//char str2[100];
	//char str3[100];

	leds_init();
	key_gpio_init();
	//timer_init();

	puts("\n\r");
	put_hex(g_a);
	puts("\n\r");
/*
	spi_init();
	oled_init();
	oled_printf(0, 0, "Shihaiyun");

	spi_flash_read_id(&pid, &vid);
	printf("spi flash pid = 0x%x, vid = 0x%x\n\r", pid, vid);
	sprintf(str1, "pid=%x vid=%x", pid, vid);
	oled_printf(2, 0, str1);

	spi_flash_init();
	spi_flash_sector_erase(4096);
	spi_flash_page_program(4096, "Zhaodonghui", 12);
	spi_flash_read_data(4096, str2, 12);
	oled_printf(4, 0, str2);
*/
	//ts_test();
	//i2c_test();
	//while (1);

	//nor_main();
	//nand_main();
	//lcd_test();
/*	optical_resister_init();

	while (1) {
		adc = adc_read(0);
		voltage = ((double)adc / 0x3ff) * 3.300;
		m0 = (unsigned int)voltage;
		n0 = (unsigned int)((voltage - m0) * 1000);

		adc = adc_read(1);
		voltage = ((double)adc / 0x3ff) * 3.300;
		m1 = (unsigned int)voltage;
		n1 = (unsigned int)((voltage - m1) * 1000);

		printf("ain0=%d.%03dv   ain1=%d.%03dv\r", m0, n0, m1, n1);
		//sprintf(str3, "ain0=%d.%03dv", m, n);
		//oled_printf(6, 0, str3);
	}
*/
/*
	unsigned char temperature;
	unsigned char humidity;

	timer_init();
	while (1) {
		if (dht11_recv(&temperature, &humidity) == 0)
			printf("temperature = %d, humidity = %d\n\r", temperature, humidity);
	}
*/
	unsigned char rom[8];
	unsigned char i;
	double temperature;

	timer_init();
	while (1) {
		ds18b20_read_temperature(&temperature);
		m0 = (unsigned int)temperature;
		n0 = (unsigned int)((temperature - m0) * 10000);
		printf("temperature = %d.%04d\n\r", m0, n0);
		mdelay(1000);
	}

	return 0;
}

/*
int main(void)
{
	unsigned char c;
	unsigned int x = 0, y = 0;
	//p_app_params p_params;
	unsigned char count = 0;

	leds_init();
	key_gpio_init();

	puts("\n\r");
	put_hex(g_a);
	puts("\n\r");

	//nor_main();
	//nand_main();
	//lcd_test();
	lcd_font_init();
	lcd_parameters_deliver(p_params);

	while (1) {
		c = getchar();

		if (c == '\n') {
			putchar('\n');
			putchar('\r');
			x = 0;
			y += 16;
		} else if (c == '\r') {
			putchar('\n');
			putchar('\r');
			x = 0;
			y += 16;
		} else {
			putchar(c);
			lcd_char8(x, y, count, c);
			x += 8;
		}
		
		if (x >= (p_params->xres - 8)) {
			x = 0;
			y += 16;
		}

		if (y >= (p_params->yres - 8)) {
			y = 0;
			x = 0;
		}
		count++;
		if (count == 255)
			count = 0;
	}

	return 0;
}
*/
/*
int main(void)
{
	leds_init();
	key_gpio_init();
	timer_init();

	puts("\n\r");
	put_hex(g_a);
	puts("\n\r");

	while (1) {
		putchar(g_char);
		g_char++;
		putchar(g_char2);
		g_char2++;
		delay(100000);

		if (g_char == ('Z' + 1))
			g_char = 'A';
		if (g_char2 == ('z' + 1))
			g_char2 = 'a';
	}
	return 0;
}


int main(void)
{
	unsigned char c;
	int ret;

	uart_init();
	printf("Hello world\n\r");
	my_printf_test();

	while (1) {
		printf("please chose whether initiate sdram <y/n>\n\r");
		c = getchar();

		if (c == '\n') {
			putchar('\r');
		}
		if (c == '\r') {
			putchar('\n');
		}

		putchar(c);
		//set_tacc_clock(c);
		if (c == 'y')
			init_sdram();
		leds();
		ret = sdram_test();
		if (ret < 0)
			printf("\n\rsdram init fail\n\r");
		else
			printf("\n\rsdram init success\n\r");
	}

	return 0;
}
*/

