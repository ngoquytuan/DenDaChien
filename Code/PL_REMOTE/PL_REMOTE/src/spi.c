#include "spi.h"

void init_SPI(void)
{
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);//SPI Enable, Master, fosc/16
	SPSR=0x00; 
}

unsigned char SPI(unsigned char data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return (SPDR);
}

void SPIBurstRead(u8 adr, u8 *ptr, u8 length)
{
	u8 i;
	if(length>1)//length must more than one
	{
		SPI(adr);
		for(i=0;i<length;i++)
		ptr[i] = SPI(0x00);
	}
}

void BurstWrite(u8 adr, u8 *ptr, u8 length)
{
	u8 i;
	if(length>1) //length must more than one
	{
		SPI(adr|0x80);
		for(i=0;i<length;i++) SPI(ptr[i]);
	}
}
