#ifndef __PLCS_GCP_PROTOCOL_H__
#define __PLCS_GCP_PROTOCOL_H__

#include "NCProtocol.h"
#include "util.h"

// ---------------------------------------------------------------------------------------------- //
#define PLCS_GCP_PROTOCOL_ID						(0x21)

#define PLCS_GCP_REQ_STATEINFO						(0x24)
#define PLCS_GCP_RES_STATEINFO						(0x25)

#define PLCS_GCP_REQ_CONTROL_REBOOT					(0x3A)

uint16 getSeqNumGeneratorForGCP();
uint8 plcs_GetGCPMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len);
uint8 plcs_GetGCPResMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len, uint16 seqNum);

// ---------------------------------------------------------------------------------------------- //
extern char token;
extern uint8 Min;
extern uint8 Hour;
extern uint8 applyingSchedule;
// ---------------------------------------------------------------------------------------------- //
void plcs_GCP_ProcessMessageFromZigbee(uint16 srcNetAddr, uint8 msg[], int length);

//GCP Handle
void plcs_GCP_HandleStateInfoReq(uint16 srcNetAddr, uint8 msg[], int length);

//GCP Send
void plcs_gcp_SendStateInfoRes(uint16 seqNum, uint16 srcId, uint8 value);
#endif
