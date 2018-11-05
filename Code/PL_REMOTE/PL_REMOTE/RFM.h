/*
 * RFM.h
 *
 * Created: 7/10/2017 3:46:54 AM
 *  Author: Angela Belnett
 */ 


#ifndef RFM_H_
#define RFM_H_

#include "MyTypeDef.h"
#include "spi.h"

/**********************************************************
**Constant define
**********************************************************/
//Define system mode
#define C_SysMode_ModuleSel 0x01                           //Module select mode
#define C_SysMode_EntrySet  0x02                           //Entry set mode
#define C_SysMode_ProduceTest  0x03                        //Produce test mode
#define C_SysMode_Modem     0x04                           //Set Modem mode(FSK /OOK /LoRa )
#define C_SysMode_FSK       0x10
#define C_SysMode_OOK       0x11
#define C_SysMode_LoRa      0x12
#define C_SysMode_TestTx    0x21                           //Work mode
#define C_SysMode_TestRx    0x22
//#define C_SysMode_Standby   0x22
#define C_SysMode_Sleep     0x23
#define C_SysMode_Set       0x24
#define C_SysMode_FSKTx     0x25
#define C_SysMode_FSKRx     0x26
#define C_SysMode_FSKTRx    0x27
#define C_SysMode_OOKTx     0x28
#define C_SysMode_OOKRx     0x29
#define C_SysMode_OOKTRx    0x2A
#define C_SysMode_LoRaTx    0x2B
#define C_SysMode_LoRaRx    0x2C
#define C_SysMode_LoRaTRx   0x2D

//Define set mode
#define C_SysMode_Set_Freq  0x51                           //Set mode
#define C_SysMode_Set_Power 0x52
#define C_SysMode_Set_Rate  0x53
#define C_SysMode_Set_Fdev  0x54
#define C_SysMode_Set_Band  0x55

//Define sent time interval
#define C_RF_SentInterval   500                            //0.5S

//Define module work mode
#define C_ModuleWorkMode_FSK     0
#define C_ModuleWorkMode_OOK     1
#define C_ModuleWorkMode_LoRa    2
#define C_ModuleWorkMode_Standby 3
#define C_ModuleWorkMode_Sleep   4
/**********************************************************
**Define display parameter
**********************************************************/
//RFM98
const u8  C_RFM98_ModuleName[]={"98  "};
const u16 C_RFM98_FreqBuf[]={434,0};
const u8  C_RFM98_RateBuf[]={12,24,48,96,0};
const u8  C_RFM98_PowerBuf[]={20,17,13,11,0};
const u8  C_RFM98_FdevBuf[]={35,0};
const u8  C_RFM98_BandBuf[]={83,0};
const u8  C_RFM98_RSSIBuf[]={25,50,70,95,0};
const u8  C_RFM98_ModemBuf[]={C_SysMode_LoRa,C_SysMode_FSK,0};
const u8  C_RFM98_SpreadFactorBuf[]={6,7,8,9,10,11,12,0};
const u16 C_RFM98_LoRaBandBuf[]={78,104,156,208,312,417,625,1250,2500,5000,0};
const u8  C_RFM98_LoRaRSSIBuf[]={40,60,90,120,0};


/**********************************************************
**Extern variable define
**********************************************************/
extern u8  gb_RxData[32];                                  //Receive data buffer
extern u8  gb_StatusTx;                                    //Tx status flag
extern u8  gb_StatusRx;                                    //Rx status flag
extern u8  gb_ErrorFlag;                                   //Error flag
extern u16 gw_TxLedTimer;                                  //Tx LED timer
extern u8  gb_TxLedCount;                                  //Tx LED count
extern u8  gb_SysTimer_1S;                                 //System timer for 1S

extern u8  gb_KeyValue;                                    //Key value
extern u16 gw_KeyDebounceTimer;                            //Debounce timer for key

extern u8  gb_BeepOnFlag;                                  //Buzzer on flag
extern u8  gb_BeepOnCount;                                 //Buzzer on count
extern u16 gw_ToneBuf[30];                                 //Tone cache
extern u8  gb_BeepTimerH;                                  //Timer reload value for buzzer
extern u8  gb_BeepTimerL;
extern u16 gw_BeepTimer;                                   //Buzzer timer

extern u16 gw_LCDFlashTimer;                               //LCD flash display timer

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


void RFM98_Running(u8 mode,u8 WorkStatus,u8 ParaChangeFlag,u8 *TxFlag,u8 *RxFlag,u8 *RSSI);



#endif /* RFM_H_ */