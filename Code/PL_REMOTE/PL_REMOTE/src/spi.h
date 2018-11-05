#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include "MyTypeDef.h"

void init_SPI(void);
unsigned char SPI(unsigned char data);
void SPIBurstRead(u8 adr, u8 *ptr, u8 length);
void BurstWrite(u8 adr, u8 *ptr, u8 length);

#endif
