#include "i2c_controller.h"

static p_i2c_controller p_i2c_controllers[I2C_CONTROLLER_NUM];
static p_i2c_controller p_i2c_con_selected;

int i2c_controller_register(p_i2c_controller controller)
{
	int cnt;

	for (cnt = 0; cnt < I2C_CONTROLLER_NUM; cnt++) {
		if (p_i2c_controllers[cnt] == NULL) {
			p_i2c_controllers[cnt] = controller;

			return 0;
		}
	}

	return -1;
}

int i2c_controller_select(const char *name)
{
	int cnt;

	for (cnt = 0; cnt < I2C_CONTROLLER_NUM; cnt++) {
		if (p_i2c_controllers[cnt] && !strcmp(p_i2c_controllers[cnt]->name, name)) {
			p_i2c_con_selected = p_i2c_controllers[cnt];

			return 0;
		}
	}

	return -1;
}

int i2c_transfer(struct i2c_msg *msgs, int num)
{
	p_i2c_con_selected->i2c_transfer(msgs, num);
}

void i2c_init(void)
{
	s3c2440_i2c_controller_add();
	i2c_controller_select("s3c2440");
	p_i2c_con_selected->init();
}

