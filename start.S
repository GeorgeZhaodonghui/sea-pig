
.text
.global _start
_start:
	b		reset
	ldr		pc,	und_address
	ldr		pc,	swi_address
	b		halt				@ abort prefetch
	b		halt				@ abort data
	b		halt				@ reserved
	ldr		pc,	irq_address	@ irq
	b		halt				@ fiq

swi_address:
	.word	do_swi

und_address:
	.word	do_und

irq_address:
	.word	do_irq

do_und:
	/* reserve registers */
	ldr		sp,	=0x34000000
	stmdb	sp!,	{r0-r12, lr}

	/* handle exception */
	mrs		r0,	cpsr
	ldr		r1,	=exception_str
	bl		exception_handle

	/* return registers */
	ldmia	sp!,	{r0-r12, pc}^

exception_str:
	.string "undefined instruction exception"
.align 4

do_swi:
	/* reserve registers */
	ldr		sp,	=0x33e00000
	stmdb	sp!,	{r0-r12, lr}
	//ldr		r4,	[lr]
	mov		r4,	lr

	/* handle exception */
	mrs		r0,	cpsr
	ldr		r1,	=swi_str
	bl		exception_handle

	sub		r0,	r4,	#4
	bl		print_swi

	/* return registers */
	ldmia	sp!,	{r0-r12, pc}^

swi_str:
	.string "software interruption"
.align 4

do_irq:
	/* reserve register */
	ldr		sp,	=0x33d00000
	sub		lr,	lr,	#4
	stmdb	sp!,	{r0-r12, lr}

	/* handle exception */
	bl		irq_handle

	/* return registers */
	ldmia	sp!,	{r0-r12, pc}^

reset:
	/* close watchdog */
	ldr		r0,	=0x53000000
	mov		r1,	#0
	str		r1,	[r0]

	/* set locktime */
	ldr		r0,	=0x4c000000
	ldr		r1,	=0xffffffff
	str		r1,	[r0]

	/* set clock divider control register */
	ldr		r0,	=0x4c000014
	ldr		r1,	=0x5
	str		r1,	[r0]

	/* set asynchronous bus mode */
	mrc	p15,	0,	r0,	c1,	c0,	0
	orr	r0,	r0,	#0xc0000000
	mcr	p15,	0,	r0,	c1,	c0,	0

	/* set pll control register */
        ldr             r0,     =0x4c000004
        ldr             r1,     =(92<<12) | (1<<4) | (1<<0)
        str             r1,     [r0]
        
	/* chose boot from nand or nor */
	mov		r0,	#0
	ldr		r1,	[r0]
	str		r0,	[r0]
	ldr		r2,	[r0]
	cmp		r0,	r2
	ldr		sp,	=0x40000000+4096
	moveq		sp,	#4096
	streq		r1,	[r0]

	/* initiate sdram */
	bl	init_sdram
	//bl	init_sdram_p

	bl	relocate

	bl	clean_bss

	/* switch mode to usr and enable irq */
	mrs		r0,	cpsr
	bic		r0,	r0,	#0x9f
	orr		r0,	r0,	#0x10
	msr		cpsr, r0
	ldr		sp,	=0x33f00000

	/* jump to sdram */
	ldr		pc,	=sdram
sdram:
	bl	uart_init

und_exception:
	.word	0xeeadc0de

	swi	0x1234

	/* branch main */
	ldr		sp,	=0x33000000
	ldr		pc,	=main

halt:
	b	halt


