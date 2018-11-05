#ifndef __DK_SPI_h__
#define __DK_SPI_h__

#include "LCD5110.h"

//My type define
typedef unsigned char		byte;
typedef unsigned int		word;
typedef unsigned long		lword;

typedef unsigned char		u8;
typedef unsigned int		u16;
typedef unsigned long		u32;

typedef signed char			s8;
typedef signed int			s16;
typedef signed long			s32;

//Hanhyeu define for manual SPI
#define	HI(x)				PORTB |= (1<<(x))		//Output high
#define	LO(x)				PORTB &= ~(1<<(x))		//Output low
#define	SCK					5						//RFMxx sck
#define	MISO				4						//RFMxx MISO
#define	MOSI				3						//RFMxx MOSI

void SPICmd8bit(unsigned char WrPara);
unsigned char SPIRead(unsigned char adr);
unsigned char SPIRead8bit(void);
void SPIWrite(unsigned int WrPara);
void SPIBurstRead(unsigned char adr, unsigned char *ptr, unsigned char length);
void BurstWrite(unsigned char adr, unsigned char *ptr, unsigned char length);

#endif







