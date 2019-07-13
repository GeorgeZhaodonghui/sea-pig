#ifndef	_FRAMEBUFFER_H
#define	_FRAMEBUFFER_H

typedef struct lcd_operations {
	void (*dot8)(int x, int y, unsigned char colour);
	void (*dot16)(int x, int y, unsigned short colour);
	void (*circle16)(int x, int y, int r, unsigned short colour);
	void (*cross16)(int x, int y, unsigned short colour);
	void (*dot24)(int x, int y, unsigned int colour);
	void (*circle24)(int x, int y, int r, unsigned int colour);
}lcd_operations, *plcd_operations;

plcd_operations lcd_hardware(void);

#endif


