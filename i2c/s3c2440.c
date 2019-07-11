#include "i2c_controller.h"

static int s3c2440_i2c_init(void)
{
	return 0;
}

static int s3c2440_i2c_transfer(struct i2c_msg *msgs, int num)
{
	return 0;
}

static i2c_controller s3c2440_i2c_controller = {
	.name			= "s3c2440",
	.init			= s3c2440_i2c_init,
	.i2c_transfer	= s3c2440_i2c_transfer,
};

void s3c2440_i2c_controller_add(void)
{
	i2c_controller_register(&s3c2440_i2c_controller);
}

