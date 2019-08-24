#include "temperature_humidity.h"
#include "../s3c2440_soc.h"
#include "../my_printf.h"
#include "delay.h"

static void dht11_output(void)
{
	GPGCON	&= ~(0x3 << (2 * 5));
	GPGCON	|= (0x1 << (2 * 5));

	return ;
}

static void dht11_input(void)
{
	GPGCON	&= ~(0x3 << (2 * 5));
	GPGCON	|= (0x0 << (2 * 5));

	return ;
}

static void dht11_set(unsigned char val)
{
	if (val)
		GPGDAT	|= (0x1 << 5);
	else
		GPGDAT	&= ~(0x1 << 5);

	return ;
}

static unsigned char dht11_get(void)
{
	if (GPGDAT & (0x1 << 5))
		return 1;
	else
		return 0;
}

static unsigned char dht11_wait(unsigned char val, int usecond)
{
	int count;

	count = usecond;
	while((dht11_get() != val) && (count > 0)) {
		udelay(1);
		count--;
	}

	if (count > 0)
		return 1;	/* get return */
	else
		return 0;	/* overtime */
}

static void dht11_init(void)
{
	dht11_output();
	dht11_set(1);
	mdelay(2000);

	return ;
}

static void dht11_end(void)
{
	dht11_output();
	dht11_set(1);

	return ;
}

static int dht11_start(void)
{
	dht11_output();
	dht11_set(0);
	mdelay(20);

	dht11_input();
	if (!dht11_wait(0, 500)) {
		printf("no ack pull down\n\r");
		return -1;
	}

	if (!dht11_wait(1, 500)) {
		printf("no ack pull up\n\r");
		return -1;
	}

	if (!dht11_wait(0, 500)) {
		printf("data fail: no ack pull down\n\r");
		return -1;
	}

	return 0;
}

static unsigned char dht11_read(void)
{
	if (!dht11_wait(0, 500)) {
		printf("read fail, no pull down\n\r");
		return -1;
	}

	if (!dht11_wait(1, 500)) {
		printf("read fail, no pull up\n\r");
		return -1;
	}

	udelay(40);
	if (dht11_get())
		return 1;
	else
		return 0;
}

static unsigned char dht11_read_byte(void)
{
	unsigned char val;
	unsigned char i;

	for (i = 0; i < 8; i++) {
		val <<= 1;
		if (dht11_read())
			val |= 0x1;
	}

	return val;
}

int dht11_recv(unsigned char *temperature, unsigned char *humidity)
{
	unsigned char temp_int, temp_dec;
	unsigned char hum_int, hum_dec;
	unsigned char check;

	dht11_init();
	dht11_start();

	hum_int = dht11_read_byte();
	hum_dec = dht11_read_byte();
	temp_int = dht11_read_byte();
	temp_dec = dht11_read_byte();
	check = dht11_read_byte();

	dht11_end();
	if ((hum_int + hum_dec + temp_int + temp_dec) == check) {
		*temperature = temp_int;
		*humidity = hum_int;
		dht11_init();
		return 0;
	} else {
		*temperature = 0;
		*humidity = 0;
		printf("check fail\n\r");
		dht11_init();
		return -1;
	}
}


