#ifdef	_TSLIB_H
#define	_TSLIB_H

int calibrate_xy_position(unsigned int *px_position, unsigned int *py_position);
int get_lcd_from_ts(int ts_x, int ts_y, int *lcd_x, int *lcd_y);

#endif

