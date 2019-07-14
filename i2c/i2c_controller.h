#ifndef	_I2C_CONTROLLER_H
#define	_I2C_CONTROLLER_H

#define I2C_CONTROLLER_NUM		32

typedef struct i2c_msg {
	unsigned int addr;
	unsigned int flag;	/* 0:write 1:read */
	unsigned int len;
	int cnt_transferred;
	unsigned char *buf;
	int err;
}i2c_msg, *p_i2c_msg;

typedef struct i2c_controller {
	const char *name;
	int (*init)(void);
	int (*i2c_transfer)(p_i2c_msg p_msgs, int num);
	void (*i2c_interrupt_handle)(int irq);
}i2c_controller, *p_i2c_controller;

int i2c_controller_register(p_i2c_controller controller);
int i2c_controller_select(const char *name);
int i2c_transfer(p_i2c_msg p_msgs, int num);
void i2c_init(void);

#endif

