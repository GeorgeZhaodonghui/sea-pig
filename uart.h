
#ifndef _uart_h
#define _uart_h

int uart_init(void);
int putchar(int c);
int getchar(void);
int puts(const char *s);
int put_hex(int hex);

#endif

