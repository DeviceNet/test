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
#define ZRMP_RES_PING					(0x01)		// Ping ����
#define ZRMP_REQ_RESET					(0x01)		// Reset
#define ZRMP_REQ_CONFIG				(0x10)		// �������� ��ȸ ��û
#define ZRMP_RES_CONFIG				(0x11)		// �������� ��ȸ ����
#define ZRMP_REG_CONF_SET				(0x11)		// �������� ����/Ȯ�� ��û
#define ZRMP_RES_CONF_SET				(0x91)		// �������� ����/Ȯ�� ����
#define ZRMP_REG_NWK_SCAN				(0x20)		// �ֺ� ��Ʈ��ũ Ȯ�� ��û
#define ZRMP_RES_NWK_SCAN				(0xA0)		// �ֺ� ��Ʈ��ũ Ȯ�� ����
#define ZRMP_REQ_NWK_INIT				(0x21)		// ��Ʈ��ũ ���� ���� ��û
#define ZRMP_RES_NWK_INIT				(0xA1)		// ��Ʈ��ũ ���� ���� ����
#define ZRMP_REQ_NWK_PERMIT			(0x22)		// ��Ʈ��ũ ���� ���� �˸� ��û
#define ZRMP_RES_NWK_PERMIT			(0xA2)		// ��Ʈ��ũ ���� ���� �˸� ����
#define ZRMP_MSG_FORMAT				(0x00)		// �޼��� �⺻ ����

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

