#ifndef __DK_SPI_h__
#define __DK_SPI_h__

#include "DK_RFM98.h"
// #include "src/glcd/glcd.h"

void SPICmd8bit(unsigned char WrPara);
void SPILCD(unsigned char WrPara);
unsigned char SPIRead(unsigned char adr);
unsigned char SPIRead8bit(void);
void SPIWrite(unsigned int WrPara);
void SPIBurstRead(unsigned char adr, unsigned char *ptr, unsigned char length);
void BurstWrite(unsigned char adr, unsigned char *ptr, unsigned char length);

#endif







