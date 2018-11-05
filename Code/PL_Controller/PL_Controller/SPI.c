#include "spi.h"

void init_SPI(void)
{
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR1);//SPI Enable, Master, fosc/64 = 125kHz
	SPSR=0x00; 
}

unsigned char SPI(unsigned char data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return (SPDR);
}



