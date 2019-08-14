#ifndef _OLED_H
#define _OLED_H

void oled_init(void);
void oled_printf(unsigned int page, unsigned int col, unsigned char *str);
void oled_clear_all(void);
void oled_clear_page(unsigned char page);

#endif

