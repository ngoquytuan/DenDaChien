#ifndef __DK_RFM_App_h__
#define __DK_RFM_App_h__

#include "DK_RFM_Define.h"
#include "DK_SPI.h"
#include "DK_RFM98.h"


/**********************************************************
**Constant define
**********************************************************/
//Define system mode
#define C_SysMode_ModuleSel				0x01                           //Module select mode
#define C_SysMode_EntrySet				0x02                           //Entry set mode
#define C_SysMode_ProduceTest			0x03                        //Produce test mode
#define C_SysMode_Modem					0x04                           //Set Modem mode(FSK /OOK /LoRa )
#define C_SysMode_FSK					0x10
#define C_SysMode_OOK					0x11
#define C_SysMode_LoRa					0x12
#define C_SysMode_TestTx				0x21                           //Work mode
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
#define C_SysMode_Set_Freq				0x51                           //Set mode
#define C_SysMode_Set_Power				0x52
#define C_SysMode_Set_Rate				0x53
#define C_SysMode_Set_Fdev				0x54
#define C_SysMode_Set_Band				0x55

//Define sent time interval
#define C_RF_SentInterval				500                            //0.5S

//Define module work mode
#define C_ModuleWorkMode_FSK			0
#define C_ModuleWorkMode_OOK			1
#define C_ModuleWorkMode_LoRa			2
#define C_ModuleWorkMode_Standby		3
#define C_ModuleWorkMode_Sleep			4

// Key value table
// #define C_KeyNone					0x00
// #define C_KeyUp						0x01
// #define C_KeyDown					0x02
// #define C_KeyLeft					0x04
// #define C_KeyRight					0x08                



/**********************************************************
**Extern variable define
**********************************************************/
extern u8  gb_RxData[32];                                  //Receive data buffer
extern u8  gb_StatusTx;                                    //Tx status flag
extern u8  gb_StatusRx;                                    //Rx status flag
extern u8  gb_ErrorFlag;                                   //Error flag
// extern u16 gw_TxLedTimer;                                  //Tx LED timer
// extern u8  gb_TxLedCount;                                  //Tx LED count
extern volatile u8  gb_SysTimer_1S;                                 //System timer for 1S

// extern u8  gb_KeyValue;                                    //Key value
// extern u16 gw_KeyDebounceTimer;                            //Debounce timer for key

// extern u8  gb_BeepOnFlag;                                  //Buzzer on flag
// extern u8  gb_BeepOnCount;                                 //Buzzer on count
// extern u16 gw_ToneBuf[30];                                 //Tone cache
// extern u8  gb_BeepTimerH;                                  //Timer reload value for buzzer
// extern u8  gb_BeepTimerL;
// extern u16 gw_BeepTimer;                                   //Buzzer timer
// 
// extern u16 gw_LCDFlashTimer;                               //LCD flash display timer

extern u8  gb_SystemMode;                                  //System mode
extern u8  gb_SysMode_Set;                                 //System mode-Set mode
extern u16 gw_RF_SentInterval;                             //Send interval
extern u16 gw_TxTimer;                                     //Send timer (time base 1ms)
extern u8  gb_ModuleWorkEnableFlag;                        //Enable module work in Tx/Rx mode
extern u8  gb_ModuleWorkMode;                              //Module work mode
extern u8  gb_RF_Module;                                   //RF module
extern u8  gb_RF_RSSI;                                     //RF RSSI signal
extern u8  gb_ParameterChangeFlag;                         //Parameter change flag

extern u8  gb_FreqBuf_Addr;                                //RF center frequency cache address
extern u8  gb_RateBuf_Addr;
extern u8  gb_PowerBuf_Addr;
extern u8  gb_FdevBuf_Addr;
extern u8  gb_BandBuf_Addr;
extern u8  gb_Modem_Addr;
//extern u8  gb_SpreadFactor_Addr;

extern u16 gw_SendDataCount;                               //Send data count
extern u16 gw_ReceiveDataCount;                            //Receive data count

/**********************************************************
**Extern function define
**********************************************************/
// void Delay_ms(u32 time);
// void TxLED_Deal(void);
// void RxLED_Deal(void);
void ModuleSelectModeEntryCheck(void);
// void MenuConfig(void);
// void BeepOn(u16 *Tone);
// void BeepOff(void);
// void PowerOnMusic(void);

// /**********************************************************
// **Tone table
// **********************************************************/
// #define SL_1    0xf100
// #define SL_2    0xf180
// #define SL_3    0xf200
// #define SL_4    0xf280
// #define SL_5    0xf300
// #define SL_6    0xf380
// #define SL_7    0xf400
// 
// #define SM_1    0xf480
// #define SM_2    0xf500
// #define SM_3    0xf580
// #define SM_4    0xf600
// #define SM_5    0xf680
// #define SM_6    0xf700
// #define SM_7    0xf780
// 
// #define SH_1    0xf800
// #define SH_2    0xf880
// #define SH_3    0xf900
// #define SH_4    0xf980
// #define SH_5    0xfa00
// #define SH_6    0xfa80
// #define SH_7    0xfb00

#endif

