#ifndef __PLCS_MDP_PROTOCOL_H__
#define __PLCS_MDP_PROTOCOL_H__

#include "NCProtocol.h"
#include "util.h"

// ---------------------------------------------------------------------------------------------- //
#define PLCS_DP_PROTOCOL_ID 			0x11

#define PLCS_DP_REQ_DIMMING	    		(0x32)
#define PLCS_DP_REQ_WATCHDOG    		(0x40)

uint16 getSeqNumGeneratorForMDP();
uint8 plcs_GetMDPMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len);
uint8 plcs_GetMDPResMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len, uint16 seqNum);

void plcs_MDP_ProcessMessageFromZigbee(uint16 srcNetAddr, uint8 msg[], int length);
void MDP_NRFSendDimmingReqToMDP(uint8 channel, uint8 dimmingLevel);
//MDP Handle
void MDP_HandleDimmingReq(uint8 msg[], int length);

//MDP Send
void MDP_SendDimmingReqToMDP(uint8 channel, uint8 dimmingLevel);
void MDP_SendSetWatchdogReqToMDP(uint8 isEnable);

/////////////////////
uint8 MDP_GetLastSendDimmingLevel(uint8 channelNum);

#endif
