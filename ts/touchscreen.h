#ifndef	_TOUCHSCREEN_H
#define	_TOUCHSCREEN_H

/* adc touch screen control register */
#define	STYLUS_DOWN_INT			(0 << 8)
#define	STYLUS_UP_INT				(1 << 8)

#define	YM_DISABLE					(0 << 7)
#define	YM_ENABLE					(1 << 7)

#define	YP_ENABLE					(0 << 6)
#define	YP_DISABLE					(1 << 6)

#define	XM_DISABLE					(0 << 5)
#define	XM_ENABLE					(1 << 5)

#define	XP_ENABLE					(0 << 4)
#define	XP_DISABLE					(1 << 4)

#define	PULL_UP_ENABLE			(0 << 3)
#define	PULL_UP_DISABLE			(1 << 3)

#define	NORMAL_PST				(0 << 2)
#define	AUTO_PST					(1 << 2)

#define	NO_OPERATION				(0 << 0)
#define	WAIT_INTERRUPT			(3 << 0)

typedef struct xy_buf {
	unsigned int x_buf[5];
	unsigned int y_buf[5];
}xy_buf, *pxy_buf;

void touchscreen_init(void);
void tc_adc_interrupt_init(void);
void deliver_xy_buf(unsigned int *px_position, unsigned int *py_position);
void read_xy_position_flag(volatile unsigned char *flag);
void read_pressure_flag(volatile unsigned char *flag);

#endif


