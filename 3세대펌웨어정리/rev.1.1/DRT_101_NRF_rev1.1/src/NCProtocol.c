#include "NCProtocol.h"
#include "ZRMProtocol.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +
 +
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

uint8 dev_Count;
uint16 ctrl_Cmd;
uint16 device_Type;
uint32 z_ieee_Id;

uint8 ncp_NetState = NCP_NET_NOT_CONNECT;
uint8 pingCount = 0;

uint8 tmp_BuffForNCP[64];
// --------------------------------------------------------------------------- //

void ncp_ProcessMessage(uint16 srcNetAddr, uint8 msg[], int length) {
	switch (msg[8]) { // Msg Type of NCP
	case NCP_RES_PING:
		ncp_ProcessPingRes(srcNetAddr, msg, length);
		break;
	case NCP_RES_REGISTER:
		ncp_ProcessRegisterRes(srcNetAddr, msg, length);
		break;
	}
}

void ncp_ProcessPingRes(uint16 srcNetAddr, uint8 msg[], int length) {
	uint16 seqNum;
	uint8 flag = 0;

	//msg[8] = messageType
	seqNum = msg[2] << 8 | msg[3];
	flag = msg[10];

	if (flag == 0x01) {
		ncp_NetState = NCP_NET_NOT_CONNECT;
		pingCount = 0;
	} else {
		pingCount = 0;
	}
}

void ncp_ProcessRegisterRes(uint16 srcNetAddr, uint8 msg[], int length) {
	int i = 0;
	uint16 nodeId;
	uint16 seqNum;
	uint16 result = 0;
	// Request Register

	//msg[8] == messageType
	seqNum = msg[2] << 8 | msg[3];
	nodeId = (uint16) ((msg[6] << 8) | (msg[7]));
	result = (uint16) ((msg[9] << 8) | (msg[10]));

	if (result == 0)
		ncp_NetState = NCP_NET_REGISTER;
	else
		ncp_NetState = NCP_NET_NOT_CONNECT;
}

////////////////////////////////

void ncp_SendPingReq() {
	int len = 0;
	uint8 payload[10];
	uint8 resultLen = 0;

	pingCount++;

	payload[len++] = NCP_REQ_PING;
	payload[len++] = 0;
	payload[len++] = 0;

	resultLen = plcs_GetNCPMessage(tmp_BuffForNCP, 0, tmp_zrmp.zrm_Id, payload,
			len);

	if(pingCount>3) {
		ncp_NetState=NCP_NET_NOT_CONNECT;
		pingCount=0;
	}

	sendData(0, tmp_BuffForNCP, resultLen);

}

void ncp_SendRegisterReq() {
	int len = 0;
	uint8 payload[10];
	uint8 resultLen = 0;

	payload[len++] = NCP_REQ_REGISTER;
	payload[len++] = (uint8) (PLCS_ZDIMMER_TYPE >> 8);
	payload[len++] = (uint8) PLCS_ZDIMMER_TYPE;
	payload[len++] = FW_VERSION;
	payload[len++] = FW_VERSION;

	resultLen = plcs_GetNCPMessage(tmp_BuffForNCP, 0, tmp_zrmp.zrm_Id, payload,
			len);
	sendData(0, tmp_BuffForNCP, resultLen);
}
//////////////////////////////

uint16 seqNumGenerator = 0;
uint16 getSeqNumGenerator() {
	return seqNumGenerator++;
}

uint8 ncp_ConnState() {
	return ncp_NetState;
}
uint8 plcs_GetNCPMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len) {
	uint8 resultLen = 0;

	uint16 seqNum = getSeqNumGenerator();

	resultMsg[resultLen++] = NCP_PROTOCOL_ID;
	resultMsg[resultLen++] = PLCS_NCP_PROTOCOL_ID;
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



//End of File

