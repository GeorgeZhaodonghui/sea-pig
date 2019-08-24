#include "delay.h"
#include "../s3c2440_soc.h"

void udelay(int usecond)
{
	int count;
	int previous_cnt;
	int current_cnt;
	int different_val;

	count		= usecond * 5;
	previous_cnt	= TCNTO0;
	while (count > 0) {
		current_cnt = TCNTO0;
		if (previous_cnt > current_cnt)
			different_val = previous_cnt - current_cnt;
		else
			different_val = previous_cnt + 60000 - current_cnt;

		count -= different_val;
		previous_cnt = current_cnt;
	}

	return ;
}

void mdelay(int msecond)
{
	udelay(msecond * 1000);

	return ;
}


