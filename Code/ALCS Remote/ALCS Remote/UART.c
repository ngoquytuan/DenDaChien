/*
uart.c
Implement ATMEGA 8 uart communication
Date created: 08/07/2009
*/


#include "uart.h"
#ifdef USE_1UART_ATMEGA
static volatile unsigned char queueUART[MAX_QUEUE];			//receiver queue
static volatile unsigned int topQueue, bottomQueue;
#endif//USE_1UART_ATMEGA
#ifdef USE_2UART_ATMEGA   
#ifdef USE_UART1
static volatile unsigned char queueUART1[MAX_QUEUE1];			//receiver queue
static volatile unsigned int topQueue1, bottomQueue1;
#endif//USE_UART1
#ifdef USE_UART2
static volatile unsigned char queueUART2[MAX_QUEUE2];
static volatile unsigned int topQueue2, bottomQueue2;
#endif//USE_UART2
#endif//USE_2UART_ATMEGA
//vars
union packInt 
{
	int word;
	unsigned char byte[2];
} pack;
//=============================================================================================
#ifdef USE_1UART_ATMEGA
//init
void initUART(void)
{

	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: On
	// USART Transmitter: On
	// USART Mode: Asynchronous

	UCSRC=0x06;   												//8 bits Data
	UBRRL = (XTAL_FREQ/(16*BAUND_RATE))-1;
	UBRRH = (((XTAL_FREQ/(16*BAUND_RATE))-1)>>8)&0xFF;  
	UCSRB = UCSRB|(1<<RXCIE)|(0<<TXCIE)|(1<<TXEN)|(1<<RXEN); 	//TxCIE_UDRIE_RxEN_TxEN_CH9_RXB8_TxB8 TXC use for RS485
	topQueue = 0;				 								//init receiver queue
	bottomQueue = 0;
}
//-------------------------------------
unsigned char readUART()										//wait until data available
{	
	unsigned char temp;
	while (topQueue == bottomQueue);
	temp = queueUART[topQueue];
	topQueue++;
	if (topQueue == MAX_QUEUE) topQueue = 0;
	return temp;
}
//-------------------------------------
void writeUART(unsigned char data)								//write UART data and wait until transmit complete
{
#ifdef USE_RS_485
	RS_485_DIRECTION_PORT |= 1<<RS_485_DIRECTION;
#endif
	while (!(UCSRA & (1<<UDRE))){}
	UDR = data;
}
//-------------------------------------
void writeUARTRaw(unsigned char data)							//write UART data and return immediately
{	
	UDR = data;
}
//-------------------------------------
void writeWordUART(int data)
{
	pack.word = data;
	writeUART(pack.byte[1]);
	writeUART(pack.byte[0]);
}
//-------------------------------------
void writeWordUARTRaw(int data)
{
	pack.word = data;
	writeUARTRaw(pack.byte[1]);
	writeUARTRaw(pack.byte[0]);
}
//-------------------------------------
void writeStringUART(const char * str)							//write ROM string
{
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART(str[i]);
		i++;
	}
}
//-------------------------------------
void writeMStringUART(char * str)								//write RAM string
{
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART(str[i]);
		i++;
	}
}
//-------------------------------------
int readWordUART()
{
	pack.byte[1] = readUART();
	pack.byte[0] = readUART();
	return pack.word;
}
//-------------------------------------
unsigned int queueLengthUART()									//check buffer size
{
	return (bottomQueue>=topQueue) ? (bottomQueue - topQueue) : ((MAX_QUEUE - topQueue) + bottomQueue);
}
//-------------------------------------
void flushUARTRX(void)											//empty RX buffer
{
	topQueue = bottomQueue;
}
ISR(USART_RXC_vect)
{ 
    queueUART[bottomQueue] = (unsigned char) UDR;
	bottomQueue++;
	if (bottomQueue == MAX_QUEUE) bottomQueue = 0;
}
#ifdef USE_RS_485
ISR(USART_TXC_vect)
{
	RS_485_DIRECTION_PORT &= ~(1<<RS_485_DIRECTION);
}
#endif
#endif
//=============================================================================================
#ifdef USE_2UART_ATMEGA
void initUART(void)
{
#ifdef USE_UART1	
	// USART0 initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART0 Receiver: On
	// USART0 Transmitter: On
	// USART0 Mode: Asynchronous
	// USART0 Baud rate: 19200
		
	UCSR0C=0x06;//8 Bit data
	UBRR0L = (unsigned char)(((long)XTAL_FREQ/((long)16*BAUND_RATE1))-1);
	UBRR0H = (unsigned char)((((long)XTAL_FREQ/((long)16*BAUND_RATE1))-1)>>8)&0xFF;
	UCSR0B = UCSR0B |(1<<RXCIE0)|(1<<TXEN0)|(1<<RXEN0); //TxCIE_UDRIE_RxEN_TxEN_CH9_RXB8_TxB8
	topQueue1 = 0;				 //init receiver queue
	bottomQueue1 = 0;
#endif//USE_UART1
#ifdef USE_UART2		
	// USART1 initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART1 Receiver: On
	// USART1 Transmitter: On
	// USART1 Mode: Asynchronous
	// USART1 Baud rate: 19200
#ifndef USE_MODBUS		
	UCSR1C	=	0x06;
#else
	UCSR1C	=	0x26;//Even parity, 8-bit data
#endif	
	UBRR1L	=	(unsigned char)(((long)XTAL_FREQ/((long)16*BAUND_RATE2))-1);
	UBRR1H	=	(unsigned char)((((long)XTAL_FREQ/((long)16*BAUND_RATE2))-1)>>8)&0xFF;
	UCSR1B	=	UCSR1B |(1<<TXCIE1)|(1<<RXCIE1)|(1<<TXEN1)|(1<<RXEN1); //TxCIE_UDRIE_RxEN_TxEN_CH9_RXB8_TxB8
	topQueue2 = 0;												//init receiver queue
	bottomQueue2 = 0;
#endif//USE_UART2
#ifdef USE_RS_485_1
	RS_485_DIRECTION_PORT1 &= ~(1<<RS_485_DIRECTION1);
#endif		
#ifdef USE_RS_485_2
	RS_485_DIRECTION_PORT2 &= ~(1<<RS_485_DIRECTION2);
#endif

}
//===============================================================
//UART1 section
#ifdef USE_UART1
unsigned char readUART1(void)									//wait until data available
{	
	unsigned char temp;
	while (topQueue1 == bottomQueue1);
	temp = queueUART1[topQueue1];
	topQueue1++;
	if (topQueue1 == MAX_QUEUE1) topQueue1 = 0;
	return temp;
}
//-------------------------------------
void writeUART1(unsigned char data)								//write UART data and wait until transmit complete
{	
#ifdef USE_RS_485_1
	RS_485_DIRECTION_PORT1 |= 1<<RS_485_DIRECTION1;
#endif	
	while (!(UCSR0A & (1<<UDRE0))){}
	UDR0 = data;
}
//-------------------------------------
void writeUART1Raw(unsigned char data)							//write UART data and return immediately
{	
	UDR0 = data;
}
//-------------------------------------
void writeWordUART1(int data)
{
	pack.word = data;
	writeUART1(pack.byte[1]);
	writeUART1(pack.byte[0]);
}
//-------------------------------------
void writeWordUART1Raw(int data)
{
	pack.word = data;
	writeUART1Raw(pack.byte[1]);
	writeUART1Raw(pack.byte[0]);
}
//-------------------------------------
void writeStringUART1(const char * str)							//write ROM string
{			
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART1(str[i]);
		i++;
	}
}
//-------------------------------------
void writeMStringUART1(char * str)						//write RAM string
{			
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART1(str[i]);
		i++;
	}
}
//-------------------------------------
int readWordUART1(void)
{
	pack.byte[1] = readUART1();
	pack.byte[0] = readUART1();
	return pack.word;
}
//-------------------------------------
unsigned int queueLengthUART1(void)								//check buffer size
{ 
	return (bottomQueue1>=topQueue1) ? (bottomQueue1-topQueue1) : ((MAX_QUEUE1 - topQueue1) + bottomQueue1);
// 	if(bottomQueue1>=topQueue1)
// 		return (bottomQueue1-topQueue1);
// 	else
// 		return ((MAX_QUEUE1 - topQueue1) + bottomQueue1);
}
//-------------------------------------
void flushUART1RX(void)											//empty RX buffer
{							
	topQueue1 = bottomQueue1;
}
#endif//USE_UART1
//===============================================================
//UART2 section
#ifdef USE_UART2
unsigned char readUART2(void)									//wait until data available
{										
	unsigned char temp;
	while (topQueue2 == bottomQueue2);
	temp = queueUART2[topQueue2];
	topQueue2++;
	if (topQueue2 == MAX_QUEUE2) topQueue2 = 0;
	return temp;
}
//-------------------------------------
void writeUART2(unsigned char data)								//write UART data and wait until transmit complete
{	
#ifdef USE_RS_485_2
	RS_485_DIRECTION_PORT2 |= 1<<RS_485_DIRECTION2;
#endif
	while (!(UCSR1A & (1<<UDRE1))){}
	UDR1 = data;
}
//-------------------------------------
void writeUART2Raw(unsigned char data)							//write UART data and return immediately
{			
	UDR1 = data;
}
//-------------------------------------
void writeWordUART2(int data)
{
	pack.word = data;
	writeUART2(pack.byte[1]);
	writeUART2(pack.byte[0]);
}
//-------------------------------------
void writeWordUART2Raw(int data)
{
	pack.word = data;
	writeUART2Raw(pack.byte[1]);
	writeUART2Raw(pack.byte[0]);
}
//-------------------------------------
void writeStringUART2(const char * str)				//write ROM string
{			
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART2(str[i]);
		i++;
	}
}
//-------------------------------------
void writeMStringUART2(char * str)						//write RAM string
{			
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART2(str[i]);
		i++;
	}
}
//-------------------------------------
int readWordUART2(void)
{
	pack.byte[1] = readUART2();
	pack.byte[0] = readUART2();
	return pack.word;
}
//-------------------------------------
unsigned int queueLengthUART2(void)								//check buffer size
{
	return (bottomQueue2>=topQueue2) ? (bottomQueue2 - topQueue2) : ((MAX_QUEUE2 - topQueue2) + bottomQueue2);
}
//-------------------------------------
void flushUART2RX(void)											//empty RX buffer
{		
	topQueue2 = bottomQueue2;
}
#endif//USE_UART2
#ifdef USE_UART1
ISR(USART0_RX_vect)												//interrupt section for UART1
{
	queueUART1[bottomQueue1] = (unsigned char) UDR0;
	bottomQueue1++;
	if (bottomQueue1 == MAX_QUEUE1) bottomQueue1 = 0;
}
#ifdef USE_RS_485_1
ISR(USART0_TX_vect)
{
	RS_485_DIRECTION_PORT1 &= ~(1<<RS_485_DIRECTION1);
}
#endif
#endif//USE_UART1
#ifndef USE_MODBUS
#ifdef USE_UART2
ISR(USART1_RX_vect)												//interrupt section for UART2
{
	queueUART2[bottomQueue2] = (unsigned char) UDR1;
	bottomQueue2++;
	if (bottomQueue2 == MAX_QUEUE2) bottomQueue2 = 0;
}
#ifdef USE_RS_485_2
ISR(USART1_TX_vect)
{
	RS_485_DIRECTION_PORT2 &= ~(1<<RS_485_DIRECTION2);
}
#endif
#endif//USE_UART2
#endif//USE_MODEBUS
#endif//USE_2UART_ATMEGA
//=============================================================================================
unsigned char toHex(char ch)
{
	if ((ch>='0')&&(ch<='9')) return (ch-'0');
	else
	switch(ch)
	{
		case 'A':case 'a':return 10;break;
		case 'B':case 'b':return 11;break;
		case 'C':case 'c':return 12;break;
		case 'D':case 'd':return 13;break;
		case 'E':case 'e':return 14;break;
		case 'F':case 'f':return 15;break;
	}
	return 0;
}
//=============================================================================================
void toStr(char *str, unsigned int num)
{
	unsigned char i=0;
	unsigned int tmp=0;

	for (i=4;i>0;i--)
	{
		tmp=num&0x000F;
		if (tmp<=9)
		str[i-1]=tmp+'0';
		else
		{
			if (tmp==10) str[i-1]='A';
			if (tmp==11) str[i-1]='B';
			if (tmp==12) str[i-1]='C';
			if (tmp==13) str[i-1]='D';
			if (tmp==14) str[i-1]='E';
			if (tmp==15) str[i-1]='F';
		}
		num=num>>4;
	}
	str[4]='\0';
}   
//=============================================================================================
void toHexStr2(char *str, unsigned char num)
{
	unsigned char i=0;
	unsigned int tmp=0;

	for (i=2;i>0;i--)
	{
		tmp=num&0x0F;
		if (tmp<=9)
		str[i-1]=tmp+'0';
		else
		{
			if (tmp==10) str[i-1]='A';
			if (tmp==11) str[i-1]='B';
			if (tmp==12) str[i-1]='C';
			if (tmp==13) str[i-1]='D';
			if (tmp==14) str[i-1]='E';
			if (tmp==15) str[i-1]='F';
		}
		num=num>>4;
	}
	str[2]='\0';
}
void toHexStr4(char *str, unsigned int num)
{
	unsigned char i=0;
	unsigned int tmp=0;

	for (i=4;i>0;i--)
	{
		tmp=num&0x000F;
		if (tmp<=9)
		str[i-1]=tmp+'0';
		else
		{
			if (tmp==10) str[i-1]='A';
			if (tmp==11) str[i-1]='B';
			if (tmp==12) str[i-1]='C';
			if (tmp==13) str[i-1]='D';
			if (tmp==14) str[i-1]='E';
			if (tmp==15) str[i-1]='F';
		}
		num=num>>4;
	}
	str[4]='\0';
}
void toDecStr(char *str, unsigned int num)
{
	unsigned char i=0;
	unsigned int tmp=0;

	for (i=3;i>0;i--)
	{
		tmp=num%10;
		str[i-1]=tmp+'0';
		num=num/10;
	}
	str[3]='\0';
}
void toDecStr2(char *str, unsigned int num)
{
	unsigned char i=0;
	unsigned int tmp=0;

	for (i=2;i>0;i--)
	{
		tmp=num%10;
		str[i-1]=tmp+'0';
		num=num/10;
	}
	str[2]='\0';
}
void toDecStr5(char *str, unsigned int num)
{
	unsigned char i=0;
	unsigned int tmp=0;

	for (i=4;i>0;i--)
	{
		tmp=num%10;
		str[i-1]=tmp+'0';
		num=num/10;
	}
	str[4]='\0';
}



