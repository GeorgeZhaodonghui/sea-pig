SECTIONS {

	. = 0x30000000;
	__start = . ;

	. = ALIGN(4);
	.text :
	{
		start.o (.text)
		*(.text)
	}

	. = ALIGN(4);
	.rodata : { *(.rodata) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	__bss_start = . ;
	.bss :
	{ *(.bss) *(.COMMON) }
	_end = . ;
}

