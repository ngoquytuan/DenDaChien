#ifndef __DK_SPI_h__
#define __DK_SPI_h__

//Hanhyeu define for manual SPI
#define		RFRESET_PORT	PORTB
#define		RFRESET			5
#define		nIRQ0			(PINB&(1<<6))			//RFMxx nIRQ0
#define		HI(x)			PORTB |= (1<<(x))		//Output high
#define		LO(x)			PORTB &= ~(1<<(x))		//Output low
#define		nCS				4						//RFMxx module CS pin
#define		SCK				1						//RFMxx sck
#define		MISO			3						//RFMxx MISO
#define		MOSI			2						//RFMxx MOSI

//#define USE_GRAPHIC_LCD
#include "DK_RFM98.h"
#ifdef USE_GRAPHIC_LCD
	#include "src/glcd/glcd.h"
#endif

void SPICmd8bit(unsigned char WrPara);
#ifdef USE_GRAPHIC_LCD
	void SPILCD(unsigned char WrPara);
#endif
unsigned char SPIRead(unsigned char adr);
unsigned char SPIRead8bit(void);
void SPIWrite(unsigned int WrPara);
void SPIBurstRead(unsigned char adr, unsigned char *ptr, unsigned char length);
void BurstWrite(unsigned char adr, unsigned char *ptr, unsigned char length);

#endif







