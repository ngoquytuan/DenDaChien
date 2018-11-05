// ========================================================
/// @file       DK_PIC16_App.c
/// @brief      Function application function for RFDK_PIC16_LCD_ver1.1
/// @version    V2.1
/// @date       2013/12/25
/// @company    HOPE MICROELECTRONICS Co., Ltd.
/// @website    http://www.hoperf.com
/// @author     Geman Deng
/// @mobile     +86-013244720618
/// @tel        0755-82973805 Ext:846
// ========================================================
#include "DK_PIC16_App.h"

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



u8  gb_RxData[32];                                         //Receive data buffer
u8  gb_StatusTx=0;                                         //Tx status flag
u8  gb_StatusRx=0;                                         //Rx status flag
u8  gb_ErrorFlag=0;                                        //Error flag
u16 gw_TxLedTimer=0;                                       //Tx LED timer
u8  gb_TxLedCount=0;                                       //Tx LED count
u8  gb_SysTimer_1S=0;                                      //System timer for 1S

u8  gb_KeyDownFlag=0;                                      //Key down flag
u8  gb_KeyValue;                                           //Key value
u16 gw_KeyDebounceTimer=0;                                 //Debounce timer for key

u8  gb_BeepOnFlag=0;                                       //Buzzer on flag
u8  gb_BeepOnCount;                                        //Buzzer on count
u16 gw_ToneBuf[30];                                        //Tone cache
u8  gb_BeepTimerH;                                         //Timer reload value for buzzer
u8  gb_BeepTimerL;
u16 gw_BeepTimer;                                          //Buzzer timer

u16 gw_LCDFlashTimer=0;                                    //LCD flash display timer

u8  gb_SystemMode=0;                                       //System mode
u8  gb_SysMode_Set;                                        //System mode-Set mode
u16 gw_RF_SentInterval;                                    //Send interval
u16 gw_TxTimer=0;                                          //Send timer (time base 1S)
u8  gb_ModuleWorkEnableFlag=0;                             //Enable module work in Tx/Rx mode
u8  gb_ModuleWorkMode=0;                                   //Module work mode
u8  gb_RF_RSSI=0;                                          //RF RSSI signal
u8  gb_FirstPowerUpFlag=0;                                 //First power flag
u8  gb_ParameterChangeFlag=0;                              //Parameter change flag

u8  *RF_ModuleName;                                        //Module name cache
u16 *RF_FreqBuf;                                           //RF center frequency cache
u8  *RF_RateBuf;                                           //RF rate cache
u8  *RF_PowerBuf;                                          //RF power cache
u8  *RF_FdevBuf;                                           //RF deviation cache
u8  *RF_BandBuf;                                           //RF bandwidth cache
u8  *RF_RSSIBuf;                                           //RF RSSI cache
u8  *RF_ModemBuf;                                          //RF modem cache
u16 *RF_LoRaBandWidthBuf;                                  //RF LoRa bandwidth cache
u8  gb_FreqBuf_Addr=0;                                     //RF center frequency cache address
u8  gb_RateBuf_Addr=0;
u8  gb_PowerBuf_Addr=0;
u8  gb_FdevBuf_Addr=0;
u8  gb_BandBuf_Addr=0;
u8  gb_Modem_Addr=0;
u8  gb_Modem_Addr_Backup=0;

//u8  gb_SpreadFactor_Addr=0;

u16 gw_SendDataCount=0;                                    //Send data count
u16 gw_ReceiveDataCount=0;                                 //Receive data count
/**********************************************************
**Name:     Delay_ms
**Function: Delay program
**Input:    time, Millisecond
**Output:   none
**********************************************************/
void Delay_ms(u32 time)
{
  u16 i;
  while(time--)
  {
    for(i = 0; i<333; i++)
    {
      ;
    } 
  }
}

/**********************************************************
**Name:     TxLED_Deal
**Function: Tx LED display deal
**Input:    none
**Output:   none
**********************************************************/
void TxLED_Deal(void)
{
  if(gb_StatusTx==1)
  {
    if(gw_TxLedTimer==0)
    {
      gw_TxLedTimer=300;
      gb_TxLedCount++;
      if(gb_TxLedCount>6)
      {
        gb_TxLedCount=0;
      }
      
//       switch(gb_TxLedCount)
//       {
//         case 1:
//          LED4 = 1; 
// 		 break;
//         case 2:
//          LED3 = LED5 = 1; 
// 		 break;
//         case 3: 
//          LED1 = LED2 = LED6 = 1; 
// 		 break;
//         case 4:
//          LED4 = 0; 
// 		 break;
//         case 5:
//          LED3 = LED5 = 0; 
// 		 break;
//         case 6:
//          LED1 = LED2 = LED6 = 0; 
// 		 break;
//         default:
//          break;
//       }
    }
  }
  else
  { 
    if(gb_SystemMode!=C_SysMode_ProduceTest)
    {          
//       LED4=LED3=LED5=LED1=LED2=LED6=0;
      gb_TxLedCount=0;
    }
  }
}

/**********************************************************
**Name:     RxLED_Deal
**Function: Rx LED display deal
**Input:    none
**Output:   none
**********************************************************/
void RxLED_Deal(void)
{
  if(gb_StatusRx==1)
  {
//     LED7=1;
  }
  else if(gb_SystemMode!=C_SysMode_ProduceTest)
  {
//     LED7=0;
  }
}

/**********************************************************
**Name:     SaveRFParameterToFlash
**Function: Save RF parameter to flash
**Input:    none
**Output:   none
**********************************************************/
void SaveRFParameterToFlash(void)
{
  u8 i;
  
  FlashErase(EEPROM_FirstAddr);
  gb_RxData[0]=0x5A;
  gb_RxData[1]=gb_FreqBuf_Addr;
  gb_RxData[2]=gb_RateBuf_Addr;
  gb_RxData[3]=gb_PowerBuf_Addr;
  gb_RxData[4]=gb_FdevBuf_Addr;
  gb_RxData[5]=gb_BandBuf_Addr;  
  gb_RxData[6]=gb_Modem_Addr;
  gb_RxData[8]=gb_SystemMode;
  FlashWrite(EEPROM_FirstAddr, gb_RxData);
  for(i=0; i<32; i++)
    gb_RxData[i] = 0;
}

/**********************************************************
**Name:     InitSystemParameter
**Function: Initial system parameter
**Input:    none
**Output:   none
**********************************************************/
void InitSystemParameter(void)
{ 
  gw_SendDataCount=0;                                      //Send data count
  gw_ReceiveDataCount=0;                                   //Receive data count
  
  if(RF_ModemBuf[1]==0 && gb_SystemMode==C_SysMode_Modem){gb_SystemMode=C_SysMode_Set;}      //Direct entry set mode when the modem mode only one 

  gw_RF_SentInterval=C_RF_SentInterval;                    //RF parameter Send interval time(ms)      
  
  RF_ModemBuf=(u8*)C_RFM98_ModemBuf;
  if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_FSK)
  {
    RF_ModuleName=(u8*)C_RFM98_ModuleName; 
    RF_FreqBuf=(u16*)C_RFM98_FreqBuf;
    RF_RateBuf=(u8*)C_RFM98_RateBuf;
    RF_PowerBuf=(u8*)C_RFM98_PowerBuf;
    RF_FdevBuf=(u8*)C_RFM98_FdevBuf;
    RF_BandBuf=(u8*)C_RFM98_BandBuf;  
    RF_RSSIBuf=(u8*)C_RFM98_RSSIBuf;
  }
  else if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_LoRa)
  {
    RF_ModuleName=(u8*)C_RFM98_ModuleName; 
    RF_FreqBuf=(u16*)C_RFM98_FreqBuf;
    RF_PowerBuf=(u8*)C_RFM98_PowerBuf;
    RF_FdevBuf=(u8*)C_RFM98_FdevBuf;
    RF_RateBuf=(u8*)C_RFM98_SpreadFactorBuf;
    RF_LoRaBandWidthBuf=(u16*)C_RFM98_LoRaBandBuf;  
    RF_RSSIBuf=(u8*)C_RFM98_LoRaRSSIBuf;
    gb_RateBuf_Addr=3;
    gb_BandBuf_Addr=7;
  }
  
  if(gb_FirstPowerUpFlag==1)                               //FirstPowerUp
  {
    if((gb_SystemMode>=C_SysMode_FSKTx && gb_SystemMode<=C_SysMode_LoRaTRx) || (gb_SystemMode>=C_SysMode_TestTx && gb_SystemMode<=C_SysMode_Sleep))
    {   
      switch(RF_ModemBuf[gb_Modem_Addr])
      {
        case C_SysMode_FSK:
          gb_ModuleWorkMode=C_ModuleWorkMode_FSK;          //Module entry FSK mode
          break;
        case C_SysMode_OOK:
          gb_ModuleWorkMode=C_ModuleWorkMode_OOK;          //Module entry OOK mode
          break;
        case C_SysMode_LoRa:
          gb_ModuleWorkMode=C_ModuleWorkMode_LoRa;         //Module entry LoRa mode
          break;
      }  
      //if(gb_SystemMode==C_SysMode_Standby){gb_ModuleWorkMode=C_ModuleWorkMode_Standby;}   //Module entry standby mode
      if(gb_SystemMode==C_SysMode_Sleep){gb_ModuleWorkMode=C_ModuleWorkMode_Sleep;}       //Module entry sleep mode
        
      gb_ModuleWorkEnableFlag=1;                           //Enable module work in Tx/Rx mode
      gb_ParameterChangeFlag=1;                            //Enalbe entry normal mode
    }
    else
    {
      if(gb_SystemMode==C_SysMode_EntrySet)
      {
        gb_SysMode_Set=C_SysMode_Set_Freq;                 //Modify SysMode set
      }
      gb_ModuleWorkEnableFlag=0;                           //Disable module work in Tx/Rx mode
      gb_ModuleWorkMode=C_ModuleWorkMode_Standby;          //Module entry stanby mode 
      gb_ParameterChangeFlag=2;  
    }
  }  
}

// /**********************************************************
// **Name:     DisplayRFParameter
// **Function: Complex display RF parameter
// **Input:    none
// **Output:   none
// **********************************************************/
// void DisplayRFParameter(void)
// { 
// //     u8 *TempDispBuf;
//     if(gb_ErrorFlag!=1)
//     {
//       switch(gb_SystemMode)                                //Flash Display
//       {
//         case C_SysMode_Modem:
//           if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_FSK)
//           {           
// //             SendStringToDispBuf(C_DispAddr_Time, (u8*)"sel-FS");  //Select FSK mode
//           }
//           else if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_OOK)
//           {           
// //             SendStringToDispBuf(C_DispAddr_Time, (u8*)"sel-OO");  //Select OOK mode
//           } 
//           else if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_LoRa)
//           {           
// //             SendStringToDispBuf(C_DispAddr_Time, (u8*)"sel-Lr");  //Select LoRa mode
//           } 
//           gb_DispFlashFlag=C_FlashDisp_Time; 
//                     
//           DispSeg_Tx=0;                                    //Clear other seg
//           DispSeg_Rx=0;
//           DispSeg_PWR=0; 
//           DispSeg_Fdev=0;     
//           DispSeg_BW=0;
//           DispSeg_P6=0;DispSeg_BR=0;
//           DispSeg_315=0;DispSeg_434=0;DispSeg_868=0;DispSeg_915=0; 
//           
//           DispSeg_HopeRF=1;
// //           SendStringToDispBuf(C_DispAddr_RFM, RF_ModuleName);DispSeg_RFM=1;    //Module name display  
//           return;
//           //break;  
//         case C_SysMode_ModuleSel:
// //           SendStringToDispBuf(C_DispAddr_Time, (u8*)"sel-RF");
//           gb_DispFlashFlag=C_FlashDisp_RFM;
//                
//           DispSeg_Tx=0;                                    //Clear other seg
//           DispSeg_Rx=0;
//           DispSeg_PWR=0; 
//           DispSeg_Fdev=0;     
//           DispSeg_BW=0;
//           DispSeg_P6=0;DispSeg_BR=0;
//           DispSeg_315=0;DispSeg_434=0;DispSeg_868=0;DispSeg_915=0; 
//           
//           DispSeg_HopeRF=1;
// //           SendStringToDispBuf(C_DispAddr_RFM, RF_ModuleName);DispSeg_RFM=1;    //Module name display  
//           return;                               
//           //break;    
//         case C_SysMode_Set:
//           TempDispBuf=(u8*)" set  ";
//           gb_DispFlashFlag=C_FlashDisp_Time;
//           break;
//         case C_SysMode_FSKTx:
//           TempDispBuf=(u8*)"FS--T ";                  
//           break;
//         case C_SysMode_FSKRx:
//           TempDispBuf=(u8*)"FS--R ";                   
//           break;
//         case C_SysMode_FSKTRx:
//           TempDispBuf=(u8*)"FS--TR";                  
//           break;
//         case C_SysMode_OOKTx:  
//           TempDispBuf=(u8*)"OO--T ";                 
//           break;
//         case C_SysMode_OOKRx: 
//           TempDispBuf=(u8*)"OO--R ";                  
//           break;
//         case C_SysMode_OOKTRx:
//           TempDispBuf=(u8*)"OO--TR";                  
//           break;
//         case C_SysMode_LoRaTx: 
//           TempDispBuf=(u8*)"LR--T ";                  
//           break;
//         case C_SysMode_LoRaRx:
//           TempDispBuf=(u8*)"LR--R ";                   
//           break;
//         case C_SysMode_LoRaTRx: 
//           TempDispBuf=(u8*)"LR--TR";                  
//           break;
//         case C_SysMode_TestTx: 
//           TempDispBuf=(u8*)" T--T ";                 
//           break;
//         case C_SysMode_TestRx:
//           TempDispBuf=(u8*)" T--R ";                  
//           break;
// //        case C_SysMode_Standby: 
// //          TempDispBuf=(u8*)"STAND ";                 
// //          break;
//         case C_SysMode_Sleep:
//           TempDispBuf=(u8*)"SLEEP ";                 
//           break;
//         case C_SysMode_EntrySet:        
//           TempDispBuf=(u8*)" set  ";
//           if(gb_SysMode_Set==C_SysMode_Set_Freq){gb_DispFlashFlag=C_FlashDisp_Freq;}
//           else if(gb_SysMode_Set==C_SysMode_Set_Power){gb_DispFlashFlag=C_FlashDisp_PWR;}
//           else if(gb_SysMode_Set==C_SysMode_Set_Rate){gb_DispFlashFlag=C_FlashDisp_BR;}  
//           else if(gb_SysMode_Set==C_SysMode_Set_Fdev){gb_DispFlashFlag=C_FlashDisp_Fdev;}  
//           else if(gb_SysMode_Set==C_SysMode_Set_Band){gb_DispFlashFlag=C_FlashDisp_BW;}       //??????
//           break;
//       }
//     }
//     else
//     {
//       TempDispBuf=(u8*)" Err  ";                           //Error display
//       gb_DispFlashFlag=C_FlashDisp_Time;
//       BeepOn((u16*)MusicTab_Error);
//     }
// //     SendStringToDispBuf(C_DispAddr_Time, TempDispBuf);
//     
//     DispSeg_HopeRF=1;
// //     SendStringToDispBuf(C_DispAddr_RFM, RF_ModuleName);DispSeg_RFM=1;    //Module name display
//     switch(RF_FreqBuf[gb_FreqBuf_Addr])                    //Frequency display
//     {
//       case 315:
//         DispSeg_315=1;DispSeg_434=0;DispSeg_868=0;DispSeg_915=0;
//         break;
//       case 434:
//         DispSeg_315=0;DispSeg_434=1;DispSeg_868=0;DispSeg_915=0;
//         break;
//       case 868:
//         DispSeg_315=0;DispSeg_434=0;DispSeg_868=1;DispSeg_915=0;
//         break;
//       case 915:
//         DispSeg_315=0;DispSeg_434=0;DispSeg_868=0;DispSeg_915=1;
//         break;
//     }
// //     SendNumberToDispBuf(C_DispAddr_BR,RF_RateBuf[gb_RateBuf_Addr]); //RF Rate display
//     if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_LoRa)         //LoRa mode
//     {
//       DispSeg_P6=0;DispSeg_BR=0;
//     }
//     else
//     {  
//       DispSeg_P6=1;DispSeg_BR=1;     
//     }
//   
//     if(RF_FdevBuf[0]==0 && RF_ModemBuf[gb_Modem_Addr]!=C_SysMode_LoRa)    //Receiver module
//     {
// //       SendStringToDispBuf(C_DispAddr_Tx,(u8*)"    ");DispSeg_Tx=0;  
// //       SendNumberToDispBuf(C_DispAddr_Rx,gw_ReceiveDataCount);DispSeg_Rx=1;
// //       SendStringToDispBuf(C_DispAddr_PWR,(u8*)"  ");DispSeg_PWR=0; 
// //       SendStringToDispBuf(C_DispAddr_Fdev,(u8*)"   ");DispSeg_Fdev=0;     
// //       SendNumberToDispBuf(C_DispAddr_BW,RF_BandBuf[gb_BandBuf_Addr]);DispSeg_BW=1;
//     }
//     else if(RF_BandBuf[0]==0 && RF_ModemBuf[gb_Modem_Addr]!=C_SysMode_LoRa)    //Transmitter module
//     {     
// //       SendNumberToDispBuf(C_DispAddr_Tx,gw_SendDataCount);DispSeg_Tx=1; 
// //       SendStringToDispBuf(C_DispAddr_Rx,(u8*)"    ");DispSeg_Rx=0;
// //       SendNumberToDispBuf(C_DispAddr_PWR,RF_PowerBuf[gb_PowerBuf_Addr]);DispSeg_PWR=1;  
// //       SendNumberToDispBuf(C_DispAddr_Fdev,RF_FdevBuf[gb_FdevBuf_Addr]);DispSeg_Fdev=1;
// //       SendStringToDispBuf(C_DispAddr_BW,(u8*)"   ");DispSeg_BW=0;
//     }
//     else                                                   //Transceiver module                                                     
//     {
// //       SendNumberToDispBuf(C_DispAddr_Tx,gw_SendDataCount);DispSeg_Tx=1; 
// //       SendNumberToDispBuf(C_DispAddr_Rx,gw_ReceiveDataCount);DispSeg_Rx=1;
// //       SendNumberToDispBuf(C_DispAddr_PWR,RF_PowerBuf[gb_PowerBuf_Addr]);DispSeg_PWR=1;   //RF out power display
//       if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_LoRa)       //LoRa mode
//       {
// //         SendStringToDispBuf(C_DispAddr_Fdev,(u8*)"   ");DispSeg_Fdev=0;        //RF Fdev display 
//         if(RF_LoRaBandWidthBuf[gb_BandBuf_Addr]<1000)      //RF BW display
//         {
// //           SendNumberToDispBuf(C_DispAddr_BW,RF_LoRaBandWidthBuf[gb_BandBuf_Addr]);DispSeg_BW=1; DispSeg_P8=1;
//         }
//         else
//         {
// //           SendNumberToDispBuf(C_DispAddr_BW,RF_LoRaBandWidthBuf[gb_BandBuf_Addr]/10);DispSeg_BW=1; DispSeg_P8=0;
//         }
//       }
//       else
//       {       
// //         SendNumberToDispBuf(C_DispAddr_Fdev,RF_FdevBuf[gb_FdevBuf_Addr]);DispSeg_Fdev=1;        //RF Fdev display 
// //         SendNumberToDispBuf(C_DispAddr_BW,RF_BandBuf[gb_BandBuf_Addr]);DispSeg_BW=1;DispSeg_P8=0;     //RF BW display       
//       }
//     }
//     
//     if(RF_RSSIBuf[0]!=0 &&(gb_SystemMode==C_SysMode_FSKRx||gb_SystemMode==C_SysMode_TestRx||gb_SystemMode==C_SysMode_LoRaRx||gb_SystemMode==C_SysMode_OOKRx))
//     {
//       DispSeg_Q5=1;                                        //RF RSSI signal display
//       if(gb_RF_RSSI!=0xff)
//       {
//         DispSeg_Q4=1;
//         DispSeg_Q3=1;
//         DispSeg_Q2=1;
//         DispSeg_Q1=1;
//         if(gb_RF_RSSI<RF_RSSIBuf[3]){DispSeg_Q1=0;}
//         if(gb_RF_RSSI<RF_RSSIBuf[2]){DispSeg_Q2=0;}
//         if(gb_RF_RSSI<RF_RSSIBuf[1]){DispSeg_Q3=0;}
//         if(gb_RF_RSSI<RF_RSSIBuf[0]){DispSeg_Q4=0;}         
//       //SendNumberToDispBuf(C_DispAddr_Tx,gb_RF_RSSI);
//       }
//     }
//     else                                                   //Transmitter module
//     {
//       DispSeg_Q5=0;DispSeg_Q4=0;DispSeg_Q3=0;DispSeg_Q2=0;DispSeg_Q1=0;
//     }
// }

/**********************************************************
**Name:     ModuleSelectModeEntryCheck
**Function: Confirm whether you can enter module select mode
**Input:    none
**Output:   none
**********************************************************/
void ModuleSelectModeEntryCheck(void)
{
  gb_FirstPowerUpFlag=1;                                   //FirstPowerUp
  FlashRead(EEPROM_FirstAddr, gb_RxData);
                        
  if(gb_RxData[0]==0x5A)                                 //Power on ,config default parameter
  {   
    gb_FreqBuf_Addr=gb_RxData[1];
    gb_RateBuf_Addr=gb_RxData[2];
    gb_PowerBuf_Addr=gb_RxData[3];
    gb_FdevBuf_Addr=gb_RxData[4];
    gb_BandBuf_Addr=gb_RxData[5]; 
    gb_Modem_Addr_Backup=gb_Modem_Addr=gb_RxData[6]; 
    gb_SystemMode=gb_RxData[8];
  }
  else
  {
    gb_FreqBuf_Addr=0;
    gb_RateBuf_Addr=1;
    gb_PowerBuf_Addr=0;
    gb_FdevBuf_Addr=0;
    gb_BandBuf_Addr=0;
    gb_Modem_Addr_Backup=gb_Modem_Addr=0;     
    
    gb_SystemMode=C_SysMode_Modem;                       //RF select modem mode
  } 
   
  InitSystemParameter();
}

/**********************************************************
**Name:     RFParameterSelect
**Function: Select RF parameter
**Input:    KeyValue, key value
**Output:   none
**********************************************************/
void RFParameterSelect(u8 KeyValue)
{ 
  if(KeyValue==C_KeyUp)                                    //Page Up key
  {
//     BeepOn((u16*)MusicTab_KeySet);
    
    gb_SysMode_Set++;
    if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_LoRa)
    {
      if(gb_SysMode_Set==C_SysMode_Set_Fdev){gb_SysMode_Set=C_SysMode_Set_Band;}
      if(gb_SysMode_Set>C_SysMode_Set_Band){gb_SysMode_Set=C_SysMode_Set_Freq;}
    }
    else
    {
      if(gb_SysMode_Set>C_SysMode_Set_Rate){gb_SysMode_Set=C_SysMode_Set_Freq;}   
      if(RF_FdevBuf[0]==0 && gb_SysMode_Set==C_SysMode_Set_Power){gb_SysMode_Set++;}
    }
  }
  else if(KeyValue==C_KeyLeft)                             //Page left key
  {
//     BeepOn((u16*)MusicTab_KeyMove);                        //buzzer on    
    
    switch(gb_SysMode_Set)
    {
      case C_SysMode_Set_Freq:
        if(gb_FreqBuf_Addr){gb_FreqBuf_Addr--;}
        else
        {
          while(1)
          {
            gb_FreqBuf_Addr++;
            if(RF_FreqBuf[gb_FreqBuf_Addr]==0)
            {
              gb_FreqBuf_Addr--;
              break;
            }
          }
        }
        break;
      case C_SysMode_Set_Power:
        gb_PowerBuf_Addr++;
        if(RF_PowerBuf[gb_PowerBuf_Addr]==0){gb_PowerBuf_Addr=0;}
        break;
      case C_SysMode_Set_Rate:
        if(gb_RateBuf_Addr){gb_RateBuf_Addr--;}
        else
        {
          while(1)
          {
            gb_RateBuf_Addr++;
            if(RF_RateBuf[gb_RateBuf_Addr]==0)
            {
              gb_RateBuf_Addr--;
              break;
            }
          }
        }
        break;
      case C_SysMode_Set_Band:                             //Only in LoRa mode
        if(gb_BandBuf_Addr){gb_BandBuf_Addr--;}
        else
        {
          while(1)
          {
            gb_BandBuf_Addr++;
            if(RF_LoRaBandWidthBuf[gb_BandBuf_Addr]==0)
            {
              gb_BandBuf_Addr--;
              break;
            }
          }
        }
        break;
    }         
  }
  else if(KeyValue==C_KeyRight)                            //Page right key
  {
//     BeepOn((u16*)MusicTab_KeyMove);                        //buzzer on
    
    switch(gb_SysMode_Set)
    {
      case C_SysMode_Set_Freq:
        gb_FreqBuf_Addr++;
        if(RF_FreqBuf[gb_FreqBuf_Addr]==0){gb_FreqBuf_Addr=0;}
        break;
      case C_SysMode_Set_Power:
        if(gb_PowerBuf_Addr){gb_PowerBuf_Addr--;}
        else
        {
          while(1)
          {
            gb_PowerBuf_Addr++;
            if(RF_PowerBuf[gb_PowerBuf_Addr]==0)
            {
              gb_PowerBuf_Addr--;
              break;
            }
          }
        }
        break;      
      case C_SysMode_Set_Rate:
        gb_RateBuf_Addr++;
        if(RF_RateBuf[gb_RateBuf_Addr]==0){gb_RateBuf_Addr=0;}
        break;    
      case C_SysMode_Set_Band:
        gb_BandBuf_Addr++;
        if(RF_LoRaBandWidthBuf[gb_BandBuf_Addr]==0){gb_BandBuf_Addr=0;}
        break;
    }
  }
  else if(KeyValue==C_KeyDown)                             //OK key
  {
//     BeepOn((u16*)MusicTab_KeyOK);                          //Set over,  Quit set mode 
    
    gb_SystemMode=C_SysMode_Set;                           //Set mode
//     SaveRFParameterToFlash(); 
  } 
}

/**********************************************************
**Name:     ModemSelect
**Function: Select module mode (FSK/OOK/LoRa)
**Input:    KeyValue, key value
**Output:   none
**********************************************************/
void ModemSelect(u8 KeyValue)
{
  if(KeyValue==C_KeyLeft)                                  //Page left key
  {
//     BeepOn((u16*)MusicTab_KeyMove);                        //buzzer on  
      
    if(gb_Modem_Addr){gb_Modem_Addr--;}
    else
    {
      while(1)
      {
        gb_Modem_Addr++;
        if(RF_ModemBuf[gb_Modem_Addr]==0)
        {
          gb_Modem_Addr--;
          break;
        }
      }
    }
  }
  else if(KeyValue==C_KeyRight)                            //Page right key
  {
//     BeepOn((u16*)MusicTab_KeyMove);                        //buzzer on
    
    gb_Modem_Addr++;
    if(RF_ModemBuf[gb_Modem_Addr]==0){gb_Modem_Addr=0;}         
  }
  else if(KeyValue==C_KeyDown)                             //OK key
  {
//     BeepOn((u16*)MusicTab_KeyOK);                          //Set over,  Quit modem set mode 
  
    gb_ParameterChangeFlag=2;                              //Module parameter changed
    gb_SystemMode=C_SysMode_Set;
    
    if(gb_Modem_Addr_Backup!=gb_Modem_Addr)                //Initialze parameter when Modem mode changed
    {       
      if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_FSK)
      {      
        gb_FreqBuf_Addr=0;
        gb_RateBuf_Addr=1;
        gb_PowerBuf_Addr=0;
        gb_FdevBuf_Addr=0;
        gb_BandBuf_Addr=0;
      }
      else if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_OOK)
      {           
      } 
      else if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_LoRa)
      {           
        gb_FreqBuf_Addr=0;
        gb_RateBuf_Addr=0;
        gb_PowerBuf_Addr=0;
        gb_FdevBuf_Addr=0;
        gb_BandBuf_Addr=0;
      } 
    }    
    InitSystemParameter();                                 //Initrupt RF parameter
//     SaveRFParameterToFlash();                              //Save Module value
  }
  else if(KeyValue!=C_KeyNone)
  {
//     BeepOn((u16*)MusicTab_KeyErr);
  } 
}

/**********************************************************
**Name:     WorkModeSelect
**Function: Select work mode
**Input:    KeyValue, key value
**Output:   none
**********************************************************/
void WorkModeSelect(u8 KeyValue)
{   
  if(KeyValue==C_KeyUp)                                    //Page Up key
  { 
    gb_ErrorFlag=0;                                        //Clear error flag  
    gb_ParameterChangeFlag=2;
    gb_ModuleWorkMode=C_ModuleWorkMode_Standby;            //Module entry stanby mode
       
    if(gb_SystemMode!=C_SysMode_Set)
    {
      gb_SystemMode=C_SysMode_Set;                         //Entry set mode
    }
    else
    {
      if(RF_ModemBuf[1]!=0)
      {   
        gb_SystemMode=C_SysMode_Modem;                     //Entry Modem mode
//         Clr_LCD();                                         //Clear LCD display
      }
      else
      {
//         BeepOn((u16*)MusicTab_KeyErr);                     //Disable entry set modem mode when the modem mode only one 
      }     
    }
    if(RF_ModemBuf[1]!=0)
    {
//       BeepOn((u16*)MusicTab_KeySet); 
      SaveRFParameterToFlash();                            //Save system mode
    }
  }
  else if(KeyValue==C_KeyLeft && gb_ParameterChangeFlag==2)    //Page left key
  {
//     BeepOn((u16*)MusicTab_KeyMove);                        //buzzer on 
     
    switch(RF_ModemBuf[gb_Modem_Addr])
    {
      case C_SysMode_FSK:
        if(gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_FSKRx+1;}                       //Not Tx&Rx mode
        gb_SystemMode--;        
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_FSKRx;}     //Only receive
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_FSKTx){gb_SystemMode=C_SysMode_Set;}  
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_TestRx){gb_SystemMode=C_SysMode_TestTx;}    //Only send
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_FSKRx){gb_SystemMode=C_SysMode_FSKTx;}
        break;
      case C_SysMode_OOK:
        if(gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_OOKRx+1;}                       //Not Tx&Rx mode
        gb_SystemMode--;   
        if(gb_SystemMode==C_SysMode_FSKTRx){gb_SystemMode=C_SysMode_Set;}     
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_OOKRx;}     //Only receive
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_OOKTx){gb_SystemMode=C_SysMode_Set;}  
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_TestRx){gb_SystemMode=C_SysMode_TestTx;}    //Only send
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_OOKRx){gb_SystemMode=C_SysMode_OOKTx;}
        break;
      case C_SysMode_LoRa:
        if(gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_LoRaRx+1;}                      //Not Tx&Rx mode
        gb_SystemMode--;   
        if(gb_SystemMode==C_SysMode_OOKTRx){gb_SystemMode=C_SysMode_Set;}     
//        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_LoRaRx;}    //Only receive
//        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_LoRaTx){gb_SystemMode=C_SysMode_Set;} 
//        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_TestRx){gb_SystemMode=C_SysMode_TestTx;}    //Only send
//        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_LoRaRx){gb_SystemMode=C_SysMode_LoRaTx;}
        break;
    } 
  }
  else if(KeyValue==C_KeyRight && gb_ParameterChangeFlag==2)    //Page right key
  {
//     BeepOn((u16*)MusicTab_KeyMove);                        //buzzer on
         
    gb_SystemMode++;
    switch(RF_ModemBuf[gb_Modem_Addr])
    {
      case C_SysMode_FSK:       
        if(gb_SystemMode>C_SysMode_FSKRx){gb_SystemMode=C_SysMode_TestTx;}  
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_TestRx;}    //Only receive
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_FSKTx){gb_SystemMode=C_SysMode_FSKRx;}  
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_TestRx){gb_SystemMode=C_SysMode_Sleep;}     //Only send
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_FSKRx){gb_SystemMode=C_SysMode_TestTx;}
        break;
      case C_SysMode_OOK: 
        if(gb_SystemMode>C_SysMode_OOKRx){gb_SystemMode=C_SysMode_TestTx;} 
        if(gb_SystemMode==C_SysMode_FSKTx){gb_SystemMode=C_SysMode_OOKTx;}   
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_TestRx;}    //Only receive
        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_OOKTx){gb_SystemMode=C_SysMode_OOKRx;}  
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_TestRx){gb_SystemMode=C_SysMode_Sleep;}     //Only send
        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_OOKRx){gb_SystemMode=C_SysMode_TestTx;}
        break;
      case C_SysMode_LoRa:
        if(gb_SystemMode>C_SysMode_LoRaRx){gb_SystemMode=C_SysMode_TestTx;} 
        if(gb_SystemMode==C_SysMode_FSKTx){gb_SystemMode=C_SysMode_LoRaTx;}    
//        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_TestTx){gb_SystemMode=C_SysMode_TestRx;}    //Only receive
//        if(RF_FdevBuf[0]==0 && gb_SystemMode==C_SysMode_LoRaTx){gb_SystemMode=C_SysMode_LoRaRx;}  
//        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_TestRx){gb_SystemMode=C_SysMode_Sleep;}     //Only send
//        if(RF_BandBuf[0]==0 && gb_SystemMode==C_SysMode_LoRaRx){gb_SystemMode=C_SysMode_TestTx;}
        break;
    } 
  }
  else if(KeyValue==C_KeyDown && gb_ParameterChangeFlag==2)
  {   
    if(gb_SystemMode==C_SysMode_Set)
    {
//       BeepOn((u16*)MusicTab_KeySet);
      gb_SystemMode=C_SysMode_EntrySet; 
      gb_SysMode_Set=C_SysMode_Set_Freq;      
    }
    else
    {
//       BeepOn((u16*)MusicTab_KeyOver);                        //Set over,  Entry work mode 
        
      switch(RF_ModemBuf[gb_Modem_Addr])                     //config work mode
      {
        case C_SysMode_FSK:
          gb_ModuleWorkMode=C_ModuleWorkMode_FSK;     
          break;
        case C_SysMode_OOK: 
          gb_ModuleWorkMode=C_ModuleWorkMode_OOK; 
          break;
        case C_SysMode_LoRa:
          gb_ModuleWorkMode=C_ModuleWorkMode_LoRa; 
          break;
      }
      //if(gb_SystemMode==C_SysMode_Standby){gb_ModuleWorkMode=C_ModuleWorkMode_Standby;} 
      if(gb_SystemMode==C_SysMode_Sleep){gb_ModuleWorkMode=C_ModuleWorkMode_Sleep;} 
        
      
//       gb_DispFlashFlag=C_FlashDisp_None;                     //Close flash display flag 
      if(gb_ParameterChangeFlag==2){gb_ParameterChangeFlag=1;}   //Module parameter changed
      gb_ModuleWorkEnableFlag=1;                             //Enable module work in Tx/Rx mode
      gw_SendDataCount=0;                                    //Send data count
      gw_ReceiveDataCount=0;                                 //Receive data count 
    }
    
//     SaveRFParameterToFlash();
  }
  else if(KeyValue!=C_KeyNone)
  {
//     BeepOn((u16*)MusicTab_KeyErr);
  } 
}




