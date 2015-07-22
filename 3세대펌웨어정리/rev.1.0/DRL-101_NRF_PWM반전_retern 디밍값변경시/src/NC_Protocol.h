#ifndef __NCP_PROTOCOL_H__
#define __NCP_PROTOCOL_H__

#include <stdio.h>
#include "Util.h"
#include "XNetProtocol.h"

#define NCP_PROTOCOL_ID			(0x80)	// Protocol ID
#define PLCS_NCP_PROTOCOL_ID 			0x01

#define NCP_REQ_PING				(0x01)	// Ping
#define NCP_RES_PING				(0x02)	// Ping 응답
#define NCP_REQ_REGISTER_BROADCAST		(0x05)
#define NCP_ASSIGN_TOKEN		  		(0x06)
#define NCP_REQ_TOKEN_RELEASE	  		(0x07)
#define NCP_RES_TOKEN_RELEASE	  		(0x08)
#define NCP_REQ_REGISTER			(0x11)	// 등록 요청
#define NCP_RES_REGISTER			(0x12)	// 등록 요청 응답
#define NCP_REQ_REGISTER_NODE		(0x13)	// 노드 등록 요청
#define NCP_RES_REGISTER_NODE		(0x14)	// 노드 등록 요청 응답
#define NCP_SEND_TIME_INFO				(0x15)

///////////////////////////////////////////////////////////////////////////////////////////
#define NCP_NET_NOT_CONNECT				0	
#define NCP_NET_REGISTER				1

#define PLCS_FW_VER						(0x01)
#define PLCS_GW_TYPE 					(0x2000)
#define PLCS_LC_100Z_TYPE				(0x2010)
#define PLCS_ZDIMMER_TYPE				(0x2040)
#define PLCS_ZSENSOR_TYPE				(0x2050)
#define PLCS_ZTEST_TYPE					(0x2070)


#endif

