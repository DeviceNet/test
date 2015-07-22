#include "MD_Protocol.h"
#include "NCProtocol.h"

uint8 tmpBuffForMDP[64];

void plcs_MDP_ProcessMessageFromZigbee(uint16 srcNetAddr, uint8 msg[],
		int length) {
	switch (msg[8]) { // Msg Type of NCP
	case PLCS_DP_REQ_DIMMING:
		MDP_HandleDimmingReq(msg, length);
		break;
	}
}

uint8 lastSendDimmingLevel[4] = { 0xff, 0xff, 0xff, 0xff };

void MDP_HandleDimmingReq(uint8 msg[], int length) {
	uint8 channelId = 0;
	uint8 dimmingLevel = 0;

	channelId = msg[11];
	dimmingLevel = msg[12];

	if (channelId < 4) {
		lastSendDimmingLevel[channelId] = dimmingLevel;
		MDP_SendDimmingReqToMDP(channelId, dimmingLevel);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Send
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MDP_SendDimmingReqToMDP(uint8 channel, uint8 dimmingLevel) {
	int len = 0;
	uint8 payload[15];
	uint8 resultLen;

	len = 0;
	payload[len++] = PLCS_DP_REQ_DIMMING;
	payload[len++] = tmp_zrmp.zrm_Id >> 8;
	payload[len++] = tmp_zrmp.zrm_Id;
	payload[len++] = 0x00;
	payload[len++] = lastSendDimmingLevel[0];
	payload[len++] = 0x01;
	payload[len++] = lastSendDimmingLevel[1];
	payload[len++] = 0x02;
	payload[len++] = lastSendDimmingLevel[2];
	payload[len++] = 0x03;
	payload[len++] = lastSendDimmingLevel[3];

	resultLen = plcs_GetMDPMessage(tmpBuffForMDP, 0, 0, payload, len);

	xcps_send_rs485(tmpBuffForMDP, resultLen);
	xcps_send_NRF24L01(tmpBuffForMDP, resultLen);
}
void MDP_TESTSendDimmingReqToMDP(uint8 channel, uint8 dimmingLevel) {
	printf("MDP 1\n\r");
	int len = 0;
	uint8 payload[10];
	uint8 resultLen;

	len = 0;
	payload[len++] = PLCS_DP_REQ_DIMMING;
	payload[len++] = tmp_zrmp.zrm_Id >> 8;
	payload[len++] = tmp_zrmp.zrm_Id;
	payload[len++] = 0x00;
	payload[len++] = dimmingLevel;
	payload[len++] = 0x01;
	payload[len++] = dimmingLevel;
	payload[len++] = 0x02;
	payload[len++] = dimmingLevel;
	payload[len++] = 0x03;
	payload[len++] = dimmingLevel;

	resultLen = plcs_GetMDPMessage(tmpBuffForMDP, 0, 0, payload, len);

	xcps_send_rs485(tmpBuffForMDP, resultLen);
	xcps_send_NRF24L01(tmpBuffForMDP, resultLen);
}

void MDP_SendSetWatchdogReqToMDP(uint8 isEnable) {
	int len = 0;
	uint8 payload[10];
	uint8 resultLen;

	len = 0;
	payload[len++] = PLCS_DP_REQ_WATCHDOG;
	payload[len++] = tmp_zrmp.zrm_Id >> 8;
	payload[len++] = tmp_zrmp.zrm_Id;
	payload[len++] = isEnable;

	resultLen = plcs_GetMDPMessage(tmpBuffForMDP, 0, 0, payload, len);

	xcps_send_rs485(tmpBuffForMDP, resultLen);
}

////////////////////////////////////

uint16 seqNumGeneratorForMDP = 0;
uint16 getSeqNumGeneratorForMDP() {
	return seqNumGeneratorForMDP++;
}

uint8 plcs_GetMDPMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len) {
	uint8 resultLen = 0;
	uint16 seqNum = getSeqNumGeneratorForMDP();

	resultMsg[resultLen++] = NCP_PROTOCOL_ID;
	resultMsg[resultLen++] = PLCS_DP_PROTOCOL_ID;
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

uint8 plcs_GetMDPResMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len, uint16 seqNum) {
	uint8 resultLen = 0;

	resultMsg[resultLen++] = NCP_PROTOCOL_ID;
	resultMsg[resultLen++] = PLCS_DP_PROTOCOL_ID;
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

uint8 MDP_GetLastSendDimmingLevel(uint8 channelNum) {
	return lastSendDimmingLevel[channelNum];
}
