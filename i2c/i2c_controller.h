#ifndef	_I2C_CONTROLLER_H
#define	_I2C_CONTROLLER_H

#define I2C_CONTROLLER_NUM		32

typedef struct i2c_msg {
	unsigned int addr;
	unsigned int flag;
	unsigned int len;
	unsigned int con_transferred;
	unsigned char *buf;
}i2c_msg, *p_i2c_msg;

typedef struct i2c_controller {
	const int *name;
	int (*init)(void);
	int (*i2c_transfer)(struct i2c_msg *msgs, int num);
}i2c_controller, *p_i2c_controller;

int i2c_controller_register(p_i2c_controller *controller);
int i2c_controller_select(const char *name);
int i2c_transfer(struct i2c_msg *msgs, int num);
void i2c_init(void);

#endif

