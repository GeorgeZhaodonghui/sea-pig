#ifndef _NAND_H
#define _NAND_H

void nand_init(void);
int nand_main(void);
int nand_read_page(unsigned int address, unsigned int len, unsigned char *content);

#endif

