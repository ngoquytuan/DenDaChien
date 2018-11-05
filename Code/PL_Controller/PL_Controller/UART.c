/*
uart.c
Implement ATMEGA 8 uart communication
Date created: 08/07/2009
*/


#include "uart.h"
    
#define BAUND_RATE 96      //*100 
#define XTAL_FREQ  80000    //*100 Hz
#define MAX_QUEUE  20

//#define BIT(x) (1<<x)

static volatile unsigned char queueUART[MAX_QUEUE];
static volatile unsigned int topQueue, bottomQueue;

//vars
union packInt {
	int word;
	unsigned char byte[2];
} pack;

//init
void initUART(void){
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
//UART
unsigned char readUART(){										//wait until data available
	unsigned char temp;
	while (topQueue == bottomQueue);
	temp = queueUART[topQueue];
	topQueue++;
	if (topQueue == MAX_QUEUE) topQueue = 0;
	return temp;
}

//-------------------------------------
void writeUART(unsigned char data){	//write UART data and wait until transmit complete
// 	PORTD |= 0x04;
	while (!(UCSRA & (1<<UDRE))){}
	UDR = data;
}

//-------------------------------------
void writeUARTRaw(unsigned char data){			//write UART data and return immediately
	UDR = data;
}

//-------------------------------------
void writeWordUART(int data){
	pack.word = data;
	writeUART(pack.byte[1]);
	writeUART(pack.byte[0]);
}

//-------------------------------------
void writeWordUARTRaw(int data){
	pack.word = data;
	writeUARTRaw(pack.byte[1]);
	writeUARTRaw(pack.byte[0]);
}

//-------------------------------------
void writeStringUART(const char * str){			//write ROM string
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART(str[i]);
		i++;
	}
}

//-------------------------------------
void writeMStringUART(char * str){			//write RAM string
	unsigned char i = 0;
	while (str[i] != 0){
		writeUART(str[i]);
		i++;
	}
}

//-------------------------------------
int readWordUART(){
	pack.byte[1] = readUART();
	pack.byte[0] = readUART();
	return pack.word;
}

//-------------------------------------
unsigned int queueLengthUART(){//check buffer size
	return (bottomQueue>=topQueue) ? (bottomQueue - topQueue) : ((MAX_QUEUE - topQueue) + bottomQueue);
}

//-------------------------------------
void flushUARTRX(void){		//empty RX buffer
	topQueue = bottomQueue;
}

ISR(USART_RXC_vect)
{ 
    queueUART[bottomQueue] = (unsigned char) UDR;
	bottomQueue++;
	if (bottomQueue == MAX_QUEUE) bottomQueue = 0;
}

ISR(USART_TXC_vect)
{
// 	PORTD &= (~0x04);
} 

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
void toDecStr(char *str, unsigned int num)
{
	unsigned char tempBuf[8];
	unsigned char i, j;
	j = 0;
	if(num < 0){
		num = -1*num;
		str[0] = '-';
		j++;
	}
	i = 0;
	do {
		tempBuf[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	while (num != 0);
	while (i>0){
		str[j] = tempBuf[i - 1];
		i--;
		j++;
	}
	str[j] = 0;
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
void toDecStr3(char *str, unsigned int num)
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
void toDecStr4(char *str, unsigned int num)
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



