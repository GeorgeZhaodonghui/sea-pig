#include "i2c_controller.h"

static int s3c2440_i2c_interrupt_handle_tx(void)
{
	IICSTAT = (0x3 << 6) | (0x0 << 5) | (0x1 << 4); /* 0x54000004 */
	IICDS   = 0xA0;	/* 0x5400000c */
	IICSTAT = 0xF0;	/* 0x54000004 */

	if (i2c_msg.len > i2c_msg.cnt_transferred) {
		IICDS = i2c_msg.buf[i2c_msg.cnt_transferred];
		i2c_msg.cnt_transferred++;
		IICON &= ~(0x1 <<4);
		sleep(10);
	} else {
		IICSTAT = 0xD0;
		IICCON  = ~(0x1 << 4);
		sleep(10);
	}

	return 0;
}

static int s3c2440_i2c_interrupt_handle(void)
{
	return 0;
}

static int s3c2440_i2c_init(void)
{
	/* IICSCL	GPE14
	 * IICSDA	GPE15
	 */
	GPECON &= ~((0x3 << 30) | (0x3 << 28));
	GPECON |= (0x2 << 30) | (0x2 << 28);	/* 0x56000040 */
	IICCON = (0x1 << 7) | (0x1 << 6) | (0x1 << 5) |
				(0x0 << 4) | (0x0 << 0);	/* 0x54000000 */

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

