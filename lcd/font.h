#ifndef _FONT_H
#define _FONT_H

int lcd_font_init(void);
int lcd_char16(int x, int y, unsigned short colour, unsigned char c);
int lcd_string16(int x, int y, unsigned short colour, unsigned char *str);
int lcd_char24(int x, int y, unsigned int colour, unsigned char c);
int lcd_string24(int x, int y, unsigned int colour, unsigned char *str);
int lcd_char8(int x, int y, unsigned char colour, unsigned char c);

#endif


