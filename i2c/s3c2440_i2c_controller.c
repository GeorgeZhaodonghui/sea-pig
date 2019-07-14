#include "i2c_controller.h"
#include "../s3c2440_soc.h"

static p_i2c_msg p_cur_msg;

static int s3c2440_i2c_interrupt_handle_tx(void)
{
	unsigned char iicstat;

	iicstat = IICSTAT;
	p_cur_msg->cnt_transferred++;

	if (iicstat & 0x1) {
		IICSTAT = 0xD0;
		IICCON  = ~(0x1 << 4);
		p_cur_msg->err = -1;
		return -1;
	}

	if (p_cur_msg->len > (p_cur_msg->cnt_transferred -1)) {
		IICDS = p_cur_msg->buf[p_cur_msg->cnt_transferred - 1];
		IICCON  = ~(0x1 << 4);
	} else {
		IICSTAT = 0xD0;
		IICCON  = ~(0x1 << 4);
	}

	return 0;
}

static int s3c2440_i2c_interrupt_handle_rx(void)
{
	unsigned char iicstat;

	iicstat = IICSTAT;
	p_cur_msg->cnt_transferred++;

	if (iicstat & 0x1) {
		IICSTAT = 0xB0;
		IICCON  = ~(0x1 << 4);
		p_cur_msg->err = -1;
		return -1;
	}

	if (p_cur_msg->len > (p_cur_msg->cnt_transferred -1)) {
		p_cur_msg->buf[p_cur_msg->cnt_transferred - 1] = IICDS;
		IICCON  = ~(0x1 << 4);
	} else {
		IICSTAT = 0x90;
		IICCON  = ~(0x1 << 4);
	}

	return 0;
}

static void s3c2440_i2c_interrupt_handle(int irq)
{
	if (p_cur_msg->flag == 0) {
		s3c2440_i2c_interrupt_handle_tx();
	} else if (p_cur_msg->flag == 1){
		s3c2440_i2c_interrupt_handle_rx();
	}

	return;
}

static int s3c2440_i2c_init(void)
{
	/* IICSCL GPE14   IICSDA GPE15 */
	GPECON &= ~((0x3 << 30) | (0x3 << 28));
	GPECON |= (0x2 << 30) | (0x2 << 28);
	
	IICCON = (0x1 << 7) | (0x1 << 6) | (0x1 << 5) |
				(0x0 << 4) | (0x0 << 0);

	return 0;
}

static int s3c2440_i2c_transfer_tx(p_i2c_msg p_msgs)
{
	p_cur_msg = p_msgs;
	
	p_msgs->cnt_transferred = 0;
	p_msgs->err = 0;

	IICSTAT = (0x3 << 6) | (0x0 << 5) | (0x1 << 4);
	IICDS   = (p_msgs->addr << 1) | 0x0;
	IICSTAT = 0xF0;

	while (!p_cur_msg->err && (p_cur_msg->len != (p_cur_msg->cnt_transferred - 1)));

	if (p_cur_msg->err)
		return -1;
	else
		return 0;
}

static int s3c2440_i2c_transfer_rx(p_i2c_msg p_msgs)
{
	p_cur_msg = p_msgs;
	
	p_msgs->cnt_transferred = 0;
	p_msgs->err = 0;
	
	IICSTAT = (0x2 << 6) | (0x0 << 5) | (0x1 << 4);
	IICDS   = (p_msgs->addr << 1) | 0x1;
	IICSTAT = 0xB0;

	while (!p_cur_msg->err && (p_cur_msg->len != p_cur_msg->cnt_transferred));

	if (p_cur_msg->err)
		return -1;
	else
		return 0;
}

static int s3c2440_i2c_transfer(p_i2c_msg p_msgs, int num)
{
	int cnt;
	int err;

	for (cnt = 0; cnt < num; cnt++) {	
		if (p_msgs->flag== 0) {
			err = s3c2440_i2c_transfer_tx(p_msgs);
		} else if (p_msgs->flag== 1){
			err = s3c2440_i2c_transfer_rx(p_msgs);
		}

		p_msgs++;
		if (err < 0)
			return -1;
	}
	return 0;
}

static i2c_controller s3c2440_i2c_controller = {
	.name				= "s3c2440",
	.init					= s3c2440_i2c_init,
	.i2c_transfer			= s3c2440_i2c_transfer,
	.i2c_interrupt_handle	= s3c2440_i2c_interrupt_handle,
};

void s3c2440_i2c_controller_add(void)
{
	i2c_controller_register(&s3c2440_i2c_controller);
}

