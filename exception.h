
#ifndef _EXCEPTION_H
#define _EXCEPTION_H

typedef void (*interrupt_handle_func)(unsigned int);

void exception_handle(unsigned int cpsr, unsigned char *str);
void print_swi(unsigned int *parameter);
void interrupt_register(interrupt_handle_func handle, int irq);

#endif

