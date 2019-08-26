#include "ds18b20.h"
#include "../s3c2440_soc.h"
#include "../my_printf.h"
#include "delay.h"

/* rom commands */
#define SEARCH_ROM				0xF0
#define READ_ROM				0x33
#define MATCH_ROM				0x55
#define SKIP_ROM				0xCC
#define ALARM_ROM				0xEC

/* functions commands */
#define CONVERT_T				0x44
#define WRITE_SCRATCHPAD		0x4E
#define READ_SCRATCHPAD		0xBE
#define COPY_SCRATCHPAD		0x48
#define RECALL_EEPROM			0xB8
#define READ_POWER_SUPPLY		0xB4

const double ds18b20_value[11] = {
	0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64};

static void ds18b20_output(void)
{
	GPGCON	&= ~(0x3 << (2 * 6));
	GPGCON	|= (0x1 << (2 * 6));

	return ;
}

static void ds18b20_input(void)
{
	GPGCON	&= ~(0x3 << (2 * 6));
	GPGCON	|= (0x0 << (2 * 6));

	return ;
}

static void ds18b20_set(unsigned char val)
{
	if (val)
		GPGDAT	|= (0x1 << 6);
	else
		GPGDAT	&= ~(0x1 << 6);

	return ;
}

static unsigned char ds18b20_get(void)
{
	if (GPGDAT & (0x1 << 6))
		return 1;
	else
		return 0;
}

static unsigned char ds18b20_wait(unsigned char val, int usecond)
{
	int count;

	count = usecond;
	while((ds18b20_get() != val) && (count > 0)) {
		udelay(1);
		count--;
	}

	udelay(5);
	if (count > 0)
		return 1;		//get return
	else
		return 0;		// overtime
}

static int initialization_timing(void)
{
	ds18b20_output();
	ds18b20_set(0);
	udelay(480);

	ds18b20_input();
	if (!ds18b20_wait(1, 60)) {
		printf("initialization_timing no pull up\n\t");
		return -1;
	}
	if (!ds18b20_wait(0, 480)) {
		printf("initialization_timing no pull down reply\n\t");
		return -1;
	}
	udelay(240);

	return 0;
}

static void ds18b20_write(unsigned char val)
{
	if (val) {
		ds18b20_output();
		ds18b20_set(0);
		udelay(2);
		ds18b20_input();
		udelay(60);
	} else {
		ds18b20_output();
		ds18b20_set(0);
		udelay(60);
		ds18b20_input();
		udelay(2);
	}

	return ;
}

static unsigned char ds18b20_read(void)
{
	unsigned char val;

	ds18b20_output();
	ds18b20_set(0);
	udelay(2);

	ds18b20_input();
	udelay(10);
	val = ds18b20_get();
	udelay(50);

	return val;
}

static unsigned char ds18b20_read_byte(void)
{
	unsigned char val = 0;
	unsigned char i;

	for (i = 0; i < 8; i++) {
		if (ds18b20_read())
			val |= (0x1 << i);
	}

	return val;
}

static void ds18b20_write_byte(unsigned char val)
{
	unsigned char i;

	for (i = 0; i < 8; i++) {
		ds18b20_write(val & (1 << i));
	}

	return ;
}

int ds18b20_read_rom(unsigned char *rom)
{
	unsigned char i;

	if (initialization_timing()) {
		printf("read rom: initialization timing error\n\r");
		return -1;
	}

	ds18b20_write_byte(READ_ROM);
	for (i = 0; i < 8; i++) {
		rom[i] = ds18b20_read_byte();
	}

	return 0;
}

int ds18b20_start_convert(void)
{
	if (initialization_timing()) {
		printf("start convert: initialization timing error\n\r");
		return -1;
	}

	ds18b20_write_byte(SKIP_ROM);
	ds18b20_write_byte(CONVERT_T);
	
	if (!ds18b20_wait(1, 1000000)) {
		printf("start convert: time out\n\r");
		return -1;
	}

	return 0;
}

int ds18b20_read_ram(unsigned char *ram, unsigned char *crc)
{
	unsigned char i;

	if (initialization_timing()) {
		printf("read ram: initialization timing error\n\r");
		return -1;
	}

	ds18b20_write_byte(SKIP_ROM);
	ds18b20_write_byte(READ_SCRATCHPAD);
	
	for (i = 0; i < 8; i++) {
		ram[i] = ds18b20_read_byte();
	}
	*crc = ds18b20_read_byte();

	return 0;
}

int ds18b20_read_temperature(double *temperature)
{
	unsigned char ram[8];
	unsigned char crc;
	unsigned char resolution;
	unsigned char i;

	if (ds18b20_start_convert()) {
		printf("ds18b20_read_temperature: start convert error\n\r");
		return -1;
	}

	if (ds18b20_read_ram(ram, &crc)) {
		printf("ds18b20_read_temperature: read ram error\n\r");
		return -1;
	}

	resolution = (ram[4] >> 5) & 0x3;
	*temperature = 0;
	for (i = (3 - resolution); i < 8; i++) {
		if (ram[0] & (0x1 << i))
			*temperature += ds18b20_value[i];
	}
	for (i = 0; i < 3; i++) {
		if (ram[1] & (0x1 << i))
			*temperature += ds18b20_value[8 + i];
	}
	if (ram[1] & (0x1 << 3))
		*temperature = 0 - *temperature;

	ds18b20_input();

	return 0;
}


