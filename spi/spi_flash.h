#ifndef _SPI_FLASH
#define _SPI_FLASH

void spi_flash_read_id(unsigned char *pid, unsigned char *vid);
void spi_flash_sector_erase(unsigned int address);
void spi_flash_page_program(unsigned int address, unsigned char *buf, unsigned int len);
void spi_flash_read_data(unsigned int address, unsigned char *buf, unsigned int len);
void spi_flash_init(void);

#endif

