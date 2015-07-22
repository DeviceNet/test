#ifndef __MSN_PROTOCOL_H__
#define __MSN_PROTOCOL_H__

#include <stdio.h>
#include "Util.h"
#include "types.h"
#include "XNetProtocol.h"

#define PLCS_DP_PROTOCOL_ID 			0x11

#define PLCS_DP_REQ_DIMMING	    		(0x32)
#define PLCS_DP_REQ_WATCHDOG    		(0x40)


extern uint8 g_DimmingLevel[4];
extern uint8 g_TestFlag;

uint8 PWM_change(uint8 data);
///////////////////////////////////////////////////////////////////////////////////////////
void MDP_ProcessMessage(uint8 msg[], int length);

void MDP_ProcessDimmingReq(uint8 msg[], int length);
void MDP_ProcessSetWatchdogReq(uint8 msg[], int length);

/////////////
uint8 MDP_IsWatchdogEnable();
uint8 MDP_GetWatchdogResetCount();
uint8 MDP_SetWatchdogResetCount(uint8 value);
#endif

