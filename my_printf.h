#ifndef _MY_PRINTF
#define _MY_PRINTF

//#include <stdio.h>
#include "uart.h"
#define	__out_putchar	putchar

#define MAX_NUMBER_BYTES	64

int my_printf_test(void);
int printf(const char *format, ...);

#endif
