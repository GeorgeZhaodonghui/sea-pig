#ifndef _GPIO_SPI_H
#define _GPIO_SPI_H

void gpio_spi_transfer_tx(unsigned char data);
unsigned char gpio_spi_transfer_rx(void);
void spi_init(void);

#endif

