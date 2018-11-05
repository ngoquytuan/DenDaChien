/*
 * LCD5110.c
 *
 * Created: 10/6/2017 9:54:44 AM
 *  Author: Angela Belnett
 */ 
#include "LCD5110.h"
#include "Font6x8.h"

void LCD_select(void)
{
	LCD_SS_set;
}
void LCD_deselect(void)
{
	LCD_SS_clr;
}

void LCD_write_byte(unsigned char dat, unsigned char command)
{
	unsigned char i;
	
	LCD_select();
	if (command == 1)
	LCD_DC_clr;
	else
	LCD_DC_set;
	
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		SDIN_set;
		else
		SDIN_clr;
		SCLK_clr;
		dat = dat << 1;
		SCLK_set;
	}
	LCD_deselect();
}

void LCD_init(void)
{
	LCD_RST_clr;
	_delay_us(1);
	LCD_RST_set;
	
	_delay_us(1);
	
	LCD_write_byte(0x21, 1); // set LCD mode
	LCD_write_byte(0xc8, 1); // set bias voltage
	LCD_write_byte(0x06, 1); // temperature correction
	LCD_write_byte(0x13, 1); // 1:48
	LCD_write_byte(0x20, 1); // use bias command, vertical
	LCD_write_byte(0x0c, 1); // set LCD mode,display normally
	LCD_clear();             // clear the LCD
}
void LCD_set_contrast(unsigned char constrast_value)
{
	LCD_write_byte(0x20 | 0x01, 1);
	LCD_write_byte(0x80 | (constrast_value&0x7F), 1);
	LCD_write_byte(0x20, 1);
	LCD_write_byte(0x08 | 0x04, 1);
}
void LCD_clear(void)
{
	unsigned int i;
	
	LCD_write_byte(0x0c, 1);
	LCD_write_byte(0x80, 1);
	
	for (i=0; i<504; i++)
	{
		LCD_write_byte(0, 0);
	}
}

void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 1); // column
	LCD_write_byte(0x80 | X, 1);    // row
}

void LCD_write_char(unsigned char c)
{
	unsigned char line;
	
	c -= 32;
	
	for (line=0; line<6; line++)
	LCD_write_byte(pgm_read_byte(&font6x8[6*c+line]), 0);
}
void LCD_write_charxy(unsigned char X,unsigned char Y, unsigned char c)
{
	LCD_set_XY(X,Y);
	LCD_write_char(c);
}
void LCD_write_string(unsigned char X,unsigned char Y,char *s)
{
	LCD_set_XY(X,Y);
	while (*s)
	{
		LCD_write_char(*s);
		s++;
	}
}

