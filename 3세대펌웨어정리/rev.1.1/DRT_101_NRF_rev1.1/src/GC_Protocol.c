#include "GC_Protocol.h"
#include "NCProtocol.h"
/////////////
uint8 tmp_BuffForGC[64];
//////////////////////////
void plcs_GCP_ProcessMessageFromZigbee(uint16 srcNetAddr, uint8 msg[], int length){
	switch (msg[8]) { // Msg Type of NCP
		case PLCS_GCP_REQ_STATEINFO:
			plcs_GCP_HandleStateInfoReq(srcNetAddr, msg, length);
			break;
		}
}

void plcs_GCP_HandleStateInfoReq(uint16 srcNetAddr, uint8 msg[], int length){
	uint16 seqNum=0;

	seqNum = msg[2] << 8 | msg[3];

	plcs_gcp_SendStateInfoRes(seqNum, tmp_zrmp.zrm_Id, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Send
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void plcs_gcp_SendStateInfoRes(uint16 seqNum, uint16 srcId, uint8 value)
{
	int resultLen = 0;
	int len=0;
	uint8 payload[32];

	payload[len++] = PLCS_GCP_RES_STATE_INFO;
	payload[len++] = 0;
	payload[len++] = 0;
	payload[len++] = srcId >> 8;
	payload[len++] = srcId;
	payload[len++] = 0x20;
	payload[len++] = 0x40;
	payload[len++] = value;

	resultLen = plcs_GetGCPResMessage(tmp_BuffForGC, 0, tmp_zrmp.zrm_Id, payload, len, seqNum);

	sendData(0x00, tmp_BuffForGC, resultLen);
}

///////////////////////////////////////////////////////////////////////////////
uint16 seqNumGeneratorForGCP = 0;
uint16 getSeqNumGeneratorForGCP() {
	return seqNumGeneratorForGCP++;
}

uint8 plcs_GetGCPMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len) {
	uint8 resultLen = 0;
	uint16 seqNum = getSeqNumGeneratorForGCP();

	resultMsg[resultLen++] = NCP_PROTOCOL_ID;
	resultMsg[resultLen++] = PLCS_GCP_PROTOCOL_ID;
	resultMsg[resultLen++] = seqNum >> 8;
	resultMsg[resultLen++] = seqNum;
	resultMsg[resultLen++] = destId >> 8;
	resultMsg[resultLen++] = destId;
	resultMsg[resultLen++] = srcId >> 8;
	resultMsg[resultLen++] = srcId;
	for (int i = 0; i < len; i++)
		resultMsg[resultLen++] = payload[i];
	return resultLen;
}

uint8 plcs_GetGCPResMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len, uint16 seqNum) {
	uint8 resultLen = 0;

	resultMsg[resultLen++] = NCP_PROTOCOL_ID;
	resultMsg[resultLen++] = PLCS_GCP_PROTOCOL_ID;
	resultMsg[resultLen++] = seqNum>>8;
	resultMsg[resultLen++] = seqNum;
	resultMsg[resultLen++] = destId >> 8;
	resultMsg[resultLen++] = destId;
	resultMsg[resultLen++] = srcId >> 8;
	resultMsg[resultLen++] = srcId;
	for (int i = 0; i < len; i++)
		resultMsg[resultLen++] = payload[i];
	return resultLen;
}


