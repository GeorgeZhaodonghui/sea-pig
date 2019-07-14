#ifndef _AT24C02_H
#define _AT24C02_H

int at24c02_write(unsigned char address, unsigned char *data, unsigned int len);
int at24c02_read(unsigned char address, unsigned char *data, unsigned int len);

#endif

