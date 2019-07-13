#include "tslib.h"
#include "../lcd/lcd.h"

static p_app_params p_params;
static unsigned char swap_enable;
static double g_kx;
static double g_ky;
static int e_ts_x;
static int e_ts_y;
static int e_lcd_x;
static int e_lcd_y;

static int is_ts_xy_swap(unsigned int *px_position, unsigned int *py_position)
{
	int dx, dy;

	dx = *(px_position + 1) - *px_position;
	dy = *(py_position + 1) - *py_position;

	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;

	if (dx > dy) {
		swap_enable = 0;
		return 0;
	} else {
		swap_enable = 1;
		return 1;
	}
}

static void swap_xy(int *px, int *py)
{
	int tmp;

	tmp = *px;
	*px = *py;
	*py = tmp;
}

int calibrate_xy_position(unsigned int *px_position, unsigned int *py_position)
{
	int ts_s1, ts_s2;
	int ts_d1, ts_d2;
	int lcd_s, lcd_d;
	int i;

	lcd_parameters_deliver(p_params);

	if (is_ts_xy_swap(px_position, py_position)) {
		for (i = 0; i < 5; i++)
			swap_xy((px_position + i), (py_position + i));
	}

	ts_s1 = *(px_position + 1) - *px_position;
	ts_s2 = *(px_position + 2) - *(px_position + 3);
	lcd_s = p_params->xres - 50 - 50;

	ts_d1 = *(py_position + 3) - *py_position;
	ts_d2 = *(py_position + 2) - *(py_position + 1);
	lcd_d = p_params->yres - 50 - 50;

	g_kx = ((double)(2 * lcd_s)) / (ts_s1 + ts_s2);
	g_ky = ((double)(2 * lcd_d)) / (ts_d1 + ts_d2);
	e_ts_x = *(px_position + 4);
	e_ts_y = *(py_position + 4);
	e_lcd_x = p_params->xres / 2;
	e_lcd_y = p_params->yres / 2;

	for (i = 0; i < 5; i++)
		printf("x = %d, y = %d\n\r", *(px_position + i), *(py_position + i));

	return 0;
}

int get_lcd_from_ts(int ts_x, int ts_y, int *lcd_x, int *lcd_y)
{
	if (swap_enable)
		swap_xy(&ts_x, &ts_y);

	*lcd_x = (int)(g_kx * (ts_x - e_ts_x) + e_lcd_x);
	*lcd_y = (int)(g_ky * (ts_y - e_ts_y) + e_lcd_y);

	return 0;
}

