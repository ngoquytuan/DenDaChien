#ifndef __DK_RFM98_h__
#define __DK_RFM98_h__

#include <util/delay.h>

// #include "Const.h"
#include "DK_RFM_App.h"
#include "UART.h"

void RFM98_Running(u8 mode,u8 WorkStatus,u8 ParaChangeFlag,u8 *TxFlag,u8 *RxFlag,u8 *RSSI);

#endif







