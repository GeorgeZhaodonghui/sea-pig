#ifndef	_DS18B20_H
#define	_DS18B20_H

int ds18b20_read_rom(unsigned char *rom);
int ds18b20_start_convert(void);
int ds18b20_read_ram(unsigned char *ram, unsigned char *crc);
int ds18b20_read_temperature(double *temperature);

#endif

