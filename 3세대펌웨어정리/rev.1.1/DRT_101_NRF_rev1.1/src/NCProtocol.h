#ifndef __NCP_PROTOCOL_H__
#define __NCP_PROTOCOL_H__

#include <stdio.h>
#include "Util.h"
#include "ZRMProtocol.h"
#include "XNetProtocol.h"


#define NCP_PROTOCOL_ID			(0x80)	// Protocol ID
#define PLCS_NCP_PROTOCOL_ID	(0x01)  // PLCS NCProtocol
#define PLCS_GCP_PROTOCOL_ID	(0x21)  // PLCS NCProtocol
#define FW_VERSION				0x1E

#define NCP_REQ_PING				(0x01)	// Ping
#define NCP_RES_PING				(0x02)	// Ping 응답
#define NCP_REQ_REGISTER			(0x11)	// 등록 요청
#define NCP_RES_REGISTER			(0x12)	// 등록 요청 응답

#define NCP_REQ_REGISTER_NODE		(0x13)	// 노드 등록 요청
#define NCP_RES_REGISTER_NODE		(0x14)	// 노드 등록 요청 응답

#define NCP_SEND_TIME_INFO				(0x15)

#define PLCS_GCP_REQ_STATE_INFO			(0X24)
#define PLCS_GCP_RES_STATE_INFO			(0X25)


#define PLCS_GW_TYPE 								(0x2000)
#define PLCS_LC_100Z_TYPE							(0x2010)
#define PLCS_ZDIMMER_TYPE							(0x2040)
#define PLCS_ZSENSOR_TYPE							(0x2050)

///////////////////////////////////////////////////////////////////////////////////////////
#define NCP_NET_NOT_CONNECT				0	
#define NCP_NET_REGISTER				1
#define NCP_NET_REBOOT					2

extern uint8 pingCounter;
extern uint8 ncp_NetState;
uint8 ncp_ConnState();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern uint8 Min;
extern uint8 Hour;

void ncp_ProcessMessage(uint16 srcAddr, uint8 msg[], int length);

void ncp_ProcessRegisterRes(uint16 zigbee_Id, uint8 msg[], int length);
void ncp_ProcessPingRes(uint16 zigbee_Id, uint8 msg[], int length);

void ncp_SendPingReq();
void ncp_SendRegisterReq();


uint8 plcs_GetNCPMessage(uint8 resultMsg[], uint16 destId, uint16 srcId,
		uint8 payload[], uint8 len);

#endif

