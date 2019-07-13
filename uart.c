#include "s3c2440_soc.h"

/* uart 8n1
/* baud rate = 50000000
  * UBRDIV = (int)(50000000 / (115200*16)) -1 = 26
  */
int uart_init(void)
{
	/* set port h control registers */
	GPHCON	&=	~((3 << 4) | (3 << 6));
	GPHCON	|=	((2 << 4) | (2 << 6));
	/* pull up GPH2,GPH3 */
	GPHUP	&=	~((1 << 2) | (1 << 3));

	/* uart line control register */
	ULCON0	=	0x00000003;
	/* uart control register */
	UCON0	=	0x00000005;
	/* uart baud rate divisor register */
	UBRDIV0	=	26;

	return 0;
}

int putchar(int c)
{
	/* whether the transmit shift register is empty */
	while (!(UTRSTAT0 & (1 << 2)));

	/* deliver the character */
	UTXH0 = (unsigned char)c;
}

int getchar(void)
{
	/* wait for receiving a data */
	while (!(UTRSTAT0 & (1 << 0)));

	/* return the character */
	return URXH0;
}

int puts(const char *s)
{
	while (*s) {
		putchar(*s);
		s++;
	}
}

int put_hex(int hex)
{
	unsigned char array[8];
	int i;

	for (i = 0; i < 8; i++) {
		array[i] = hex & 0xf;
		hex = hex >> 4;
	}

	for (i = 7; i >= 0; i--) {
		if (array[i] <= 0x9)
			putchar(array[i] + '0');
		else if (array[i] >= 0xa && array[i] <= 0xf) 
			putchar(array[i] - 0xa + 'A');
	}

	return 0;
}

