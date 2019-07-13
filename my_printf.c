/*
 * int printf(const char *format, ...);
 * int vprintf(const char *format, va_list ap);
 */
#include "my_printf.h"

typedef	char *	va_list;
#define	_INTSIZEOF(n)	( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define	va_start(ap,v)	( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t)	( *(t *)( ap = ap + _INTSIZEOF(t), ap - _INTSIZEOF(t) ) )
#define va_end(ap)	( ap = (va_list)0 )

unsigned char hex_tab[] = {'0', '1', '2', '3', '4', '5', '6', '7',\
			'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static int outc(int c)
{
	__out_putchar(c);

	return 0;
}

static int outs(const char *s)
{
	while (*s != '\0') {
		outc(*s++);
	}

	return 0;
}

/* out_num(va_arg(ap, int), 10, lead, maxwidth); */
static int out_num(long value, int base, char lead, int maxwidth)
{
	unsigned long ovalue;
	char  buf[MAX_NUMBER_BYTES];
	char *str = buf + sizeof(buf);
	int   count = 0;
	int   i;

	*--str = '\0';

	ovalue = (value > 0)? value : -value;
	do {
		*--str = hex_tab[ovalue%base];
		count++;
	} while ((ovalue /= base) != 0);

	if (maxwidth && count < maxwidth) {
		for (i = maxwidth - count; i; i--) {
			*--str = lead;
		}
	}

	if (value < 0)
		*--str = '-';

	return outs(str);
}

static int my_vprintf(const char *format, va_list ap)
{
	char	lead;
	int	maxwidth;

	for (; *format != '\0'; format++) {
		if (*format != '%') {
			outc(*format);
			continue;
		}

		lead = ' ';
		maxwidth = 0;
		/* format: %08d, %8d, %d, %u, %x, %f, %c, %s */
		format++;
		if (*format == '0') {
			lead = '0';
			format++;
		}

		/* the maximum of maxwidth is 99 */
		while (*format >= '0' && *format <= '9') {
			maxwidth *= 10;
			maxwidth += (*format - '0');
			format++;
		}

		/* chose the format of string */
		switch (*format) {
			case 'd':
				out_num(va_arg(ap, int), 10, lead, maxwidth);
				break;
			case 'o':
				out_num(va_arg(ap, unsigned int), 8, lead, maxwidth);
				break;
			case 'u':
				out_num(va_arg(ap, unsigned int), 10, lead, maxwidth);
				break;
			case 'x':
				out_num(va_arg(ap, unsigned int), 16, lead, maxwidth);
				break;
			case 'c':
				outc(va_arg(ap, int));
				break;
			case 's':
				outs(va_arg(ap, char *));
				break;
			default:
				outc(*format);
				break;
		}
	}

	return 0;
}

int printf(const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	my_vprintf(format, ap);
	va_end(ap);

	return 0;
}

int my_printf_test(void)
{
	printf("This is my_printf test\n\r");
	printf("test char           =%c,%c\n\r", 'A','a');
	printf("test decimal number =%d\n\r",    123456);
	printf("test decimal number =%d\n\r",    -123456);
	printf("test hex     number =0x%x\n\r",  0x55aa55aa);
	printf("test string         =%s\n\r",    "abcdefghijklmnopqrstuvwxyz");
	printf("num=%08d\n\r",   12345);
	printf("num=%8d\n\r",    12345);
	printf("num=0x%08x\n\r", 0x12345);
	printf("num=0x%8x\n\r",  0x12345);
	printf("num=0x%02x\n\r", 0x1);
	printf("num=0x%2x\n\r",  0x1);

	printf("num=%05d\n\r", 0x1);
	printf("num=%5d\n\r",  0x1);

	return 0;
}

