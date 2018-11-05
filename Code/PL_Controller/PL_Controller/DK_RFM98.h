#ifndef __DK_RFM98_H__
#define __DK_RFM98_H__

#include <avr/io.h>
#include <util/delay.h>
#include "DK_SPI.h"

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
#define		nIRQ0			(PIND&(1<<0))			//RFMxx nIRQ0
#define		HI(x)			PORTB |= (1<<(x))		//Output high
#define		LO(x)			PORTB &= ~(1<<(x))		//Output low
#define		nCS				0						//RFMxx module CS pin
#define		SCK				5						//RFMxx sck
#define		MISO			4						//RFMxx MISO
#define		MOSI			3						//RFMxx MOSI

/**********************************************************
**Constant define
**********************************************************/
//Define system mode
#define C_SysMode_ModuleSel				0x01	//Module select mode
#define C_SysMode_EntrySet				0x02	//Entry set mode
#define C_SysMode_ProduceTest			0x03	//Produce test mode
#define C_SysMode_Modem					0x04	//Set Modem mode(FSK /OOK /LoRa )
#define C_SysMode_FSK					0x10
#define C_SysMode_OOK					0x11
#define C_SysMode_LoRa					0x12
#define C_SysMode_TestTx				0x21	//Work mode
#define C_SysMode_TestRx				0x22
//#define C_SysMode_Standby				0x22
#define C_SysMode_Sleep					0x23
#define C_SysMode_Set					0x24
#define C_SysMode_FSKTx					0x25
#define C_SysMode_FSKRx					0x26
#define C_SysMode_FSKTRx				0x27
#define C_SysMode_OOKTx					0x28
#define C_SysMode_OOKRx					0x29
#define C_SysMode_OOKTRx				0x2A
#define C_SysMode_LoRaTx				0x2B
#define C_SysMode_LoRaRx				0x2C
#define C_SysMode_LoRaTRx				0x2D

//Define set mode
#define C_SysMode_Set_Freq				0x51	//Set mode
#define C_SysMode_Set_Power				0x52
#define C_SysMode_Set_Rate				0x53
#define C_SysMode_Set_Fdev				0x54
#define C_SysMode_Set_Band				0x55

//Define sent time interval
#define C_RF_SentInterval				500	//0.5S

//Define module work mode
#define C_ModuleWorkMode_FSK			0
#define C_ModuleWorkMode_OOK			1
#define C_ModuleWorkMode_LoRa			2
#define C_ModuleWorkMode_Standby		3
#define C_ModuleWorkMode_Sleep			4
/**********************************************************
**Extern variable define
**********************************************************/
extern u8  RFM98Data[21];
extern u8  gb_RxData[32];					//Receive data buffer
extern u8  gb_StatusTx;						//Tx status flag
extern u8  gb_StatusRx;						//Rx status flag
extern u8  gb_ErrorFlag;					//Error flag
extern volatile u8  gb_SysTimer;			//System timer for 1S

extern u8  gb_SystemMode;					//System mode
extern u8  gb_SysMode_Set;					//System mode-Set mode
extern u16 gw_RF_SentInterval;				//Send interval
extern u16 gw_TxTimer;						//Send timer (time base 1ms)
extern u8  gb_ModuleWorkEnableFlag;			//Enable module work in Tx/Rx mode
extern u8  gb_ModuleWorkMode;				//Module work mode
extern u8  gb_RF_Module;					//RF module
extern u8  gb_RF_RSSI;						//RF RSSI signal
extern u8  gb_ParameterChangeFlag;			//Parameter change flag

extern u8  gb_FreqBuf_Addr;					//RF center frequency cache address
extern u8  gb_RateBuf_Addr;
extern u8  gb_PowerBuf_Addr;
extern u8  gb_FdevBuf_Addr;
extern u8  gb_BandBuf_Addr;
extern u8  gb_Modem_Addr;
//extern u8  gb_SpreadFactor_Addr;

extern u16 gw_SendDataCount;				//Send data count
extern u16 gw_ReceiveDataCount;				//Receive data count
/**********************************************************
**Extern function define
**********************************************************/
void ModuleSelectModeEntryCheck(void);
void InitSystemParameter(void);
void RFM98_Running(u8 mode,u8 WorkStatus,u8 ParaChangeFlag,u8 *TxFlag,u8 *RxFlag,u8 *RSSI);
void RFM_RequestToSend(void);
void RFM_CheckStatus(void);
void RFM_SwitchModeCheck(u8 *TxFlag,u8 *RxFlag);
void Init_RFM(void);

#endif







