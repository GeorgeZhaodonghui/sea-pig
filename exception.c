#include "exception.h"
#include "uart.h"
#include "s3c2440_soc.h"

interrupt_handle_func interrupt_handle[32];

void exception_handle(unsigned int cpsr, unsigned char *str)
{
	puts("\n\rthe cpsr = ");
	put_hex(cpsr);
	puts("\n\r");
	puts(str);
}

void print_swi(unsigned int *parameter)
{
	puts("\n\rswi parameter = ");
	put_hex(*parameter & ~0xff000000);
	puts("\n\r");
}

static void interrupt_controller_init(int irq)
{
	/* source pending register */
	//SRCPND |= ((1 << 0) | (1 << 2) | (1 << 5));

	/* interrupt mode register */
	INTMOD &= ~(1 << irq);

	/* interrput mask register */
	INTMSK &= ~(1 << irq);

	/* interrput pending register */
	//INTPND  |= ((1 << 0) | (1 << 2) | (1 << 5));

	/* INTOFFSET */
}

void irq_handle(void)
{
	unsigned int offset_bit;

	offset_bit = INTOFFSET;
	interrupt_handle[offset_bit](offset_bit);
	//if ((offset_bit == 0) || (offset_bit == 2) || (offset_bit == 5))
	//	key_handle(offset_bit);
	//else if (offset_bit == 10)
	//	timer_handle();
	//put_hex(offset_bit);
	//EINTPEND = tmp;

	SRCPND = (1 << offset_bit);
	INTPND = (1 << offset_bit);
}

void interrupt_register(interrupt_handle_func handle, int irq)
{
	interrupt_handle[irq] = handle;

	interrupt_controller_init(irq);
}

