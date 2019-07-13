#include "s3c2440_soc.h"
#include "nand.h"
//#include "uart.h"
//#include "my_printf.h"

#define SDRAM_ADDRESS		0x30000000
/*
static int set_bankcon0(int tacc)
{
	BANKCON0 = (tacc << 8);

	return 0;
}

int set_tacc_clock(unsigned char clocks)
{
	int	tacc;
	unsigned char *clk_tab[] = {"1", "2", "3", "4", "6",\
							"8", "10", "14"};

	if (clocks >= '0' && clocks <= '7') {
		tacc = clocks - '0';
		set_bankcon0(tacc);
		printf("\n\rthe clocks is set to %s\n\r", clk_tab[tacc]);
	} else {
		printf("\n\rinvalid clocks\n\r");
	}

	return 0;
}
*/
/* set_bankcon0(int tacc); and init_sdram cannot be used at the same time. */
int init_sdram(void)
{
	BWSCON		= 0x22000000;	/* determines data bus width for bank6,7 to 32-bit */
	BANKCON6	= 0x00018701;	/* determine the memory type for bank6,7 to sdram */
	BANKCON7	= 0x00018701;	/* set Trcd = 2 clocks = 20 ns, set column = 9-bit */
	REFRESH		= 0x008404f5;	/* enable refresh, Trp = 2 clocks, Tsrc = 5 clocks, counter = 1269 */
	BANKSIZE	= 0x000000b1;	/* enable SCKE, SCLK,64M*/
	MRSRB6		= 0x00000020;	/* CAS latency = 2 clocks */
	MRSRB7		= 0x00000020;
	
	return 0;
}

int init_sdram_p(void)
{
	unsigned int array[] = {
		0x22000000, 	//BWSCON
		0x00000700, 	//BANKCON0
		0x00000700, 	//BANKCON1
		0x00000700, 	//BANKCON2
		0x00000700, 	//BANKCON3	
		0x00000700, 	//BANKCON4
		0x00000700, 	//BANKCON5
		0x00018001, 	//BANKCON6
		0x00018001, 	//BANKCON7
		0x008404f5, 	//REFRESH,HCLK=12MHz:0x008e07a3,HCLK=100MHz:0x008e04f4
		0x000000b1,	//BANKSIZE
		0x00000020,	//MRSRB6
		0x00000020,	//MRSRB7
	};
	unsigned int *p = (unsigned int *)0x48000000;
	int i;

	for (i = 0; i < 13; i++) {
		*p = array[i];
		p++;
	}

	return 0;
}

int sdram_test(void)
{
	volatile unsigned char *p = (volatile unsigned char *)SDRAM_ADDRESS;
	int i;

	for (i = 0; i < 1000; i++) {
		p[i] = 0x55;
	}

	for (i = 0; i < 1000; i++) {
		if (p[i] != 0x55)
			return -1;
	}

	return 0;
}

static int IsBootFromNand(void)
{
	unsigned int reserve;
	unsigned int *address = (unsigned int *)0;

	reserve = *address;
	*address = 0x12345678;
	if (*address == 0x12345678) {
		/* It is nand flash */
		*address = reserve;
		return 1;
	} else {
		/* It is nor flash */
		return 0;
	}
}

void relocate(void)
{
	extern unsigned int __start, __bss_start;

	volatile unsigned int *src 	= (volatile unsigned int *)0;
	volatile unsigned int *dest	= (volatile unsigned int *)&__start;
	volatile unsigned int *end	= (volatile unsigned int *)&__bss_start;

	unsigned int len;
	len = ((int)&__bss_start) - ((int)&__start);

	if (IsBootFromNand()) {
		nand_init();
		nand_read_page((unsigned int)src, len, (unsigned char *)dest);
	} else {
		while (dest < end) {
			*dest++ = *src++;
		}
	}
}

void clean_bss(void)
{
	extern unsigned int __bss_start, _end;

	volatile unsigned int *start	= (volatile unsigned int *)&__bss_start;
	volatile unsigned int *end	= (volatile unsigned int *)&_end;

	while (start <= end) {
		*start++ = 0;
	}
}


