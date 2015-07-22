#ifndef __XCP_SERIAL_H__
#define __XCP_SERIAL_H__
/**
	XCP serial protocol transport layer.
	
	시리얼 전송 프로토콜..
		- 센서노드(주로 지그비 노드)와
		  act 노드간의 시리얼 전송 프로토콜임..
	packet format = { 0xFA, len(1), <payload>, cs(1), 0xAF };

	2009/07/22. version 1.0, zigbee serial format을 수정하여 만듦.
		기본적인 포맷은 같음.
*/

#include "Util.h"
#include "Uart.h"

#define	XCPS_MAX_PDU	64

/**
	init uart.
	- just set getter/putter to use.
	- note: uart setting is done by uart module.
*/
int xcps_init_zigbee(usart_getter getter, usart_putter putter);
int xcps_init_rs485(usart_getter getter, usart_putter putter);



/**
	As version 1.1 format. any data (control or user msg).
*/
int xcps_send_zigbee(const uint8 *data, int length);
int xcps_send_rs485(const uint8 *data, int length);

int xcps_send_NRF24L01(const uint8 *data, uint8 length);
/**
	at128 recv packet..
	02/12.
	- no wait : check if ready and return none zero if ready.
	return
		0	: packet is not ready yet.
		n	: length of packet received.

	ncp_recv_uart0_v10
*/
int xcps_recv_zigbee(uint8 *buff, int buff_length);
int xcps_recv_rs485(uint8 *buff, int buff_length);

int xcps_getOriginalCode(uint8 *resultBuff, uint8 *buff, int buff_length);
int xcps_getTrnalatedCode(uint8 *resultBuff, uint8 *buff, int buff_length);

#endif
