#ifndef __ZRM_PROTOCOL_H__
#define __ZRM_PROTOCOL_H__


#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>				// For Used printf_P(PSTR());
#include "Util.h"

// ---------------------------------------------------------------------------------------------- //
#define MAX_TX_BUFF					32			// Buffer Size

#define ZRMP_PROTOCOL_ID				(0x00)		// Protocol ID
#define ZRMP_REQ_PING					(0x00)		// Ping
#define ZRMP_RES_PING					(0x01)		// Ping 응답
#define ZRMP_REQ_RESET					(0x01)		// Reset
#define ZRMP_REQ_CONFIG				(0x10)		// 설정정보 조회 요청
#define ZRMP_RES_CONFIG				(0x11)		// 설정정보 조회 응답
#define ZRMP_REG_CONF_SET				(0x11)		// 설정정보 설정/확인 요청
#define ZRMP_RES_CONF_SET				(0x91)		// 설정정보 설정/확인 응답
#define ZRMP_REG_NWK_SCAN				(0x20)		// 주변 네트워크 확인 요청
#define ZRMP_RES_NWK_SCAN				(0xA0)		// 주변 네트워크 확인 응답
#define ZRMP_REQ_NWK_INIT				(0x21)		// 네트워크 구성 시작 요청
#define ZRMP_RES_NWK_INIT				(0xA1)		// 네트워크 구성 시작 응답
#define ZRMP_REQ_NWK_PERMIT			(0x22)		// 네트워크 구성 정보 알림 요청
#define ZRMP_RES_NWK_PERMIT			(0xA2)		// 네트워크 구성 정보 알림 응답
#define ZRMP_MSG_FORMAT				(0x00)		// 메세지 기본 포맷

#define ZRMP_REQ_PANID_SET			(0x30)
#define ZRMP_REQ_CONFIG_SET			(0x40)


#define ZRM_INIT							0
#define ZRM_PING_COMPLETE				1
#define ZRM_CONFIG_COMPLETE			2
#define ZRM_CONNECT					3
// ---------------------------------------------------------------------------------------------- //


typedef struct ZRMP_Info
{
	uint16 zrm_Id;
	uint8 zrm_State;
	uint16 zrm_Panid;
	uint8 zrm_Channel;
}zrmp_Info;


extern zrmp_Info tmp_zrmp;



// --------------- Message Handler --------------- //
void ZRMPMessage(uint8 tmp_Buff[], int buff_length);



// --------------- Control Message --------------- //
void ZRMsendPing();
void ZRMsendConfig();

void ZRMSendReset();
void ZRMSendSetPanID(uint16 panId);
void ZRMSendSetChannel(uint8 channel);
void ZRMSendSetPreconfig(uint8 preconfig);

uint8 getState();

void sendData(uint16 dst_Addr, uint8 msg[], int length);
void sendData_Remote(uint16 dst_Addr, uint8 msg[], int length);

#endif

