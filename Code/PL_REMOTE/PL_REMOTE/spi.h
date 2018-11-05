#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void init_SPI(void);
unsigned char SPI(unsigned char data);

#endif
