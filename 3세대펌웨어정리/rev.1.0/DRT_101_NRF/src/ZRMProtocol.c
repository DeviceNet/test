
#include "ZRMProtocol.h"
#include "rotary.h"

uint8 tmp_Buff[32];
zrmp_Info tmp_zrmp;

#define PanIDEnable 1


void ZRMPMessage(uint8 buff[], int buff_length)
{
	switch(buff[0])
	{
		case ZRMP_RES_PING:
			// Response Ping			
			tmp_zrmp.zrm_Id = (uint16)(buff[11] << 8) | (buff[12]);
			tmp_zrmp.zrm_Panid = (uint16)(buff[13] << 8) | (buff[14]);
			if(PanIDEnable==1){
				if(tmp_zrmp.zrm_Panid != (0x0100 | rotary_GetValue())){
					tmp_zrmp.zrm_State = ZRM_INIT;
					ZRMSendSetPanID(0x0100 | rotary_GetValue());
					ZRMSendReset();
				}
				else{
					//if(tmp_zrmp.zrm_State!=ZRM_CONNECT)	
					{
						tmp_zrmp.zrm_State = ZRM_PING_COMPLETE;			
						ZRMsendConfig();
					}
				}
			}else {
				if(tmp_zrmp.zrm_State!=ZRM_CONNECT){
					tmp_zrmp.zrm_State = ZRM_PING_COMPLETE;			
					ZRMsendConfig();
				}
			}
			
			break;
			
		case ZRMP_RES_CONFIG:
			// Response Config
			if(PanIDEnable==1){
				tmp_zrmp.zrm_Channel = buff[9];

				if(tmp_zrmp.zrm_Channel != (rotary_GetValue()%16 + 11)) {
					ZRMSendSetChannel(rotary_GetValue()%16 + 11);
					ZRMsendConfig();
				}else
					tmp_zrmp.zrm_State = ZRM_CONNECT;
			}else
				tmp_zrmp.zrm_State = ZRM_CONNECT;

			break;

		case ZRMP_RES_NWK_INIT:
#ifdef DEBUG_ENABLE
			printf("\nZigbee Module Initalize");
#endif			
			ZRMsendPing();
			break;
			
		default:
			// ERROR
			break;
	}
}

uint8 getState()
{
	return tmp_zrmp.zrm_State;
}

void ZRMsendPing()
{
	int len = 0;
	
	tmp_Buff[len++] = ZRMP_PROTOCOL_ID;
	tmp_Buff[len++] = ZRMP_REQ_PING;
	
	xcps_send_zigbee(tmp_Buff, len);
	MSLEEP(3);
}

void ZRMsendConfig()
{
	int len = 0;

	tmp_Buff[len++] = ZRMP_PROTOCOL_ID;
	tmp_Buff[len++] = ZRMP_REQ_CONFIG;
	tmp_Buff[len++] = ZRMP_MSG_FORMAT;
	
	xcps_send_zigbee(tmp_Buff, len);	
	MSLEEP(3);
}

void ZRMSendReset(){
	int len = 0;
	
	tmp_Buff[len++] = ZRMP_PROTOCOL_ID;
	tmp_Buff[len++] = ZRMP_REQ_RESET;
	tmp_Buff[len++] = ZRMP_REQ_RESET;
	
	xcps_send_zigbee(tmp_Buff, len);
	MSLEEP(3);
}

void ZRMSendSetPanID(uint16 panId){
	int len = 0;

	tmp_Buff[len++] = ZRMP_PROTOCOL_ID;
	tmp_Buff[len++] = ZRMP_REQ_PANID_SET;
	tmp_Buff[len++] = panId >> 8;
	tmp_Buff[len++] = (panId&0xff);

	
	xcps_send_zigbee(tmp_Buff, len);	
	MSLEEP(3);
}

void ZRMSendSetChannel(uint8 channel){
	int len = 0;

	tmp_Buff[len++] = ZRMP_PROTOCOL_ID;
	tmp_Buff[len++] = ZRMP_REQ_CONFIG_SET;
	tmp_Buff[len++] = 0x02;
	tmp_Buff[len++] = (channel&0xff);

	
	xcps_send_zigbee(tmp_Buff, len);	
	MSLEEP(3);
}

void ZRMSendSetPreconfig(uint8 preconfig){
	int len = 0;

	tmp_Buff[len++] = ZRMP_PROTOCOL_ID;
	tmp_Buff[len++] = ZRMP_REQ_CONFIG_SET;
	tmp_Buff[len++] = 0x05;
	tmp_Buff[len++] = (preconfig&0xff);

	
	xcps_send_zigbee(tmp_Buff, len);	
	MSLEEP(3);
}


//
void sendData(uint16 dst_Addr, uint8 msg[], int length)
{
	int len = 0;
	int resultLen;
	int i=0;
	uint8 tmp_Buff[64];	
	uint8 resultBuff[64];

	tmp_Buff[len++] = 0x10;
	tmp_Buff[len++] = (uint8)(dst_Addr >> 8);
	tmp_Buff[len++] = (uint8)(dst_Addr);
	tmp_Buff[len++] = (uint8)(tmp_zrmp.zrm_Id >> 8);
	tmp_Buff[len++] = (uint8)(tmp_zrmp.zrm_Id);
	for(i=0; i < length; i++)
		tmp_Buff[len++] = msg[i];
	
	//(uint8 *resultBuff, uint8 *buff, int buff_length){
	resultLen = xcps_getTrnalatedCode(resultBuff, tmp_Buff, len);
		
	xcps_send_zigbee(resultBuff, resultLen);
	MSLEEP(3);
}


void sendData_Remote(uint16 dst_Addr, uint8 msg[], int length)
{
	int len = 0;

	tmp_Buff[len++] = 0x11;
	tmp_Buff[len++] = (uint8)(dst_Addr >> 8);
	tmp_Buff[len++] = (uint8)(dst_Addr);
	tmp_Buff[len++] = (uint8)(tmp_zrmp.zrm_Id >> 8);
	tmp_Buff[len++] = (uint8)(tmp_zrmp.zrm_Id);

	xcps_send_zigbee(tmp_Buff, len);
	MSLEEP(3);
}

//end of file


