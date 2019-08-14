#ifndef _OLED_H
#define _OLED_H

void oled_init(void);
void oled_printf(unsigned int page, unsigned int col, unsigned char *str);

#endif

