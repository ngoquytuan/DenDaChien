/*
uart.h
Implement uart communication
Date created: 24/07/2008
*/

#ifndef _UART_INCLUDED_
#define _UART_INCLUDED_     

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
/*
//USART control and status register A 
#define RXC   0x80
#define TXC   0x40
#define UDRE  0x20
#define FE    0x10
#define DOR   0x08
#define UPE   0x04
#define U2X   0x02
#define MPCM  0x01 
//USART control and status register B
#define RXCIE 0x80
#define TXCIE 0x40
#define UDRIE 0x20
#define RXEN  0x10
#define TXEN  0x08
#define UCSZ  0x04
#define RXB8  0x02
#define TXB8  0x01 
*/

void initUART(void);
//UART
unsigned char readUART();				//wait until data available
int readWordUART();
void writeUART(unsigned char data);			//write UART data and wait until transmit complete
void writeUARTRaw(unsigned char data);			//write UART data and return immediately
void writeWordUART(int data);
void writeWordUARTRaw(int data);
void writeStringUART(const char * str);			//write ROM string
void writeMStringUART(char * str);			//write RAM string
unsigned int queueLengthUART();			//check buffer size
void flushUARTRX(void);				//empty RX buffer 

unsigned char toHex(char ch);
void toStr(char *str, unsigned int num);
void toDecStr(char *str, unsigned int num);
void toDecStr2(char *str, unsigned int num);
void toDecStr3(char *str, unsigned int num);
void toDecStr4(char *str, unsigned int num);

#endif 
