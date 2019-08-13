#include "i2c_controller.h"
#include "at24c02.h"

#define 	AT24C02_I2C_ADDRESS		0x50

int at24c02_write(unsigned char address, unsigned char *data, unsigned int len)
{
	struct i2c_msg msg;
	int cnt;
	unsigned char buf[2];

	for (cnt = 0; cnt < len; cnt++) {
		buf[0]		= address++;
		buf[1]		= data[cnt];

		msg.addr		= AT24C02_I2C_ADDRESS;
		msg.flag		= 0;
		msg.len		= 2;
		msg.buf		= buf;
		msg.err		= 0;
		msg.cnt_transferred	= -1;

		i2c_transfer(&msg, 1);
	}
	
	return 0;
}

int at24c02_read(unsigned char address, unsigned char *data, unsigned int len)
{
	struct i2c_msg msg[2];

		msg[0].addr	= AT24C02_I2C_ADDRESS;
		msg[0].flag	= 0;
		msg[0].len	= 1;
		msg[0].buf	= &address;
		msg[0].err	= 0;
		msg[0].cnt_transferred	= -1;

		msg[1].addr	= AT24C02_I2C_ADDRESS;
		msg[1].flag	= 1;
		msg[1].len	= len;
		msg[1].buf	= data;
		msg[1].err	= 0;
		msg[1].cnt_transferred	= -1;

		i2c_transfer((p_i2c_msg)&msg, 2);
	
	return 0;
}
