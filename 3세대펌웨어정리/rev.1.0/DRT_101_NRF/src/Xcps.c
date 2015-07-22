#include <stdio.h>
#include <string.h>
#include "Xcps.h"
#include "Uart.h"

#include "NRF24L01/NRF24L01.h"
// protocol message format start/end flag.
#define	ZS_SFLAG	0xFA
#define	ZS_EFLAG	0xAF
extern uint8_t RX_BUF[];
extern uint8_t TX_BUF[];

/**
 temporary rx packet.
 */
uint8 xcps_rx_packet_zigbee[XCPS_MAX_PDU];
uint8 xcps_rx_packet_rs485[XCPS_MAX_PDU];

// 03/01. xcp zigbee uart transport receive state.
static int xcps_state_zigbee = 0;
int xcps_rxlen_zigbee = 0; // total length received.
int xcps_pdu_len_zigbee = 0;

static int xcps_state_rs485 = 0;
int xcps_rxlen_rs485 = 0; // total length received.
int xcps_pdu_len_rs485 = 0; // packet payload length.

/**
 uart interface to use.
 */
static usart_TxEnabler xcps_TxEnabler_zigbee = (usart_TxEnabler) 0;
static usart_TxDisabler xcps_TxDisabler_zigbee = (usart_TxDisabler) 0;

static usart_TxEnabler xcps_TxEnabler_rs485 = (usart_TxEnabler) 0;
static usart_TxDisabler xcps_TxDisabler_rs485 = (usart_TxDisabler) 0;

static usart_getter xcps_getter_zigbee = (usart_getter) 0;
static usart_putter xcps_putter_zigbee = (usart_putter) 0;

static usart_getter xcps_getter_rs485 = (usart_getter) 0;
static usart_putter xcps_putter_rs485 = (usart_putter) 0;

/**

 **/
int xcps_init_zigbee(usart_getter getter, usart_putter putter) {
	xcps_getter_zigbee = getter;
	xcps_putter_zigbee = putter;

	return 0;
}

int xcps_init_rs485(usart_getter getter, usart_putter putter) {
	xcps_getter_rs485 = getter;
	xcps_putter_rs485 = putter;

	//xcps_TxDisabler_rs485();

	return 0;
}

/**

 */
int xcps_send_zigbee(const uint8 *data, int length) {
	uint8 buff[XCPS_MAX_PDU];
	int i, msgLen = 0;
	int checkSum = 0;

	// check putter.
	if (!xcps_putter_zigbee)
		return -1;

	buff[msgLen++] = ZS_SFLAG; //Start Byte_1
	buff[msgLen++] = (uint8) (length); //Dest Addr

	for (i = 0; i < length; i++)
		buff[msgLen++] = data[i];

	// calc checksum.
	for (i = 2; i < msgLen; i++)
		checkSum += ((uint8) buff[i] & 0xFF);

	buff[msgLen++] = (uint8) (checkSum & 0xFF);

	// end flag.
	buff[msgLen++] = ZS_EFLAG;
	for (i = 0; i < msgLen; i++)
		xcps_putter_zigbee(buff[i]);

	return 0;
}

int xcps_recv_zigbee(uint8 *buff, int buff_length) {
	uint8 temp;
	int i;
	int checkSum = 0;
	int len = 0;

	// check getter.
	if (!xcps_getter_zigbee)
		return -1;

	if (xcps_getter_zigbee(&temp) < 1)
		return 0;

	if (xcps_rxlen_zigbee >= buff_length)
		xcps_rxlen_zigbee = 0;

	if (temp == ZS_SFLAG) {
		xcps_rxlen_zigbee = 0;
		xcps_rx_packet_zigbee[xcps_rxlen_zigbee++] = temp;
	} else if (temp == ZS_EFLAG) {
		uint8 len = 0;
		uint8 resultLen = 0;
		uint8 resultBuff[64];

		//getOriginalCode from translated code
		len = xcps_getOriginalCode(resultBuff, &xcps_rx_packet_zigbee[1],
				xcps_rxlen_zigbee - 1) - 2;

		//length
		if (len != resultBuff[0])
			return 0;
		//checksum		
		for (i = 1; i < len; i++)
			checkSum += ((uint8) resultBuff[i] & 0xFF);
		//if(checkSum!=resultBuff[len-1]) return 0;

		for (i = 1; i <= len; i++) {
			buff[resultLen++] = resultBuff[i];
		}
		//xcps_send_zigbee(xcps_rx_packet_zigbee, resultLen);

		//MSLEEP(3);
		return resultLen;
	} else {
		xcps_rx_packet_zigbee[xcps_rxlen_zigbee++] = temp;
	}
	return 0;
}

// -------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------- //
int xcps_send_rs485(const uint8 *data, int length) {
	uint8 buff[XCPS_MAX_PDU];
	int i, msgLen = 0;
	int checkSum = 0;

	// check putter.
	if (!xcps_putter_rs485)
		return -1;

	buff[msgLen++] = ZS_SFLAG; //Start Byte_1
	buff[msgLen++] = (uint8) (length);

	for (i = 0; i < length; i++)
		buff[msgLen++] = data[i];

	// calc checksum.
	for (i = 2; i < msgLen; i++)
		checkSum += ((uint8) buff[i] & 0xFF);

	buff[msgLen++] = (uint8) (checkSum & 0xFF);

	// end flag.
	buff[msgLen++] = ZS_EFLAG;
	//xcps_TxEnabler_rs485();
	//MSLEEP(1);
	for (i = 0; i < msgLen; i++)
		xcps_putter_rs485(buff[i]);
	//xcps_TxDisabler_rs485();

	return 0;
}
int xcps_send_NRF24L01(const uint8 *data, uint8 length) {
	uint8 buff[XCPS_MAX_PDU];
	int i, msgLen = 0;
	uint8 checkSum = 0;

	// check putter.
//	if (!xcps_putter_rs485)
//		return -1;

	TX_BUF[msgLen++] = ZS_SFLAG; //Start Byte_1
	TX_BUF[msgLen++] = (uint8) (length);

	for (i = 0; i < length; i++)
		TX_BUF[msgLen++] = data[i];

	// calc checksum.
	for (i = 2; i < msgLen; i++)
		checkSum += ((uint8) TX_BUF[i] & 0xFF);

	TX_BUF[msgLen++] = (uint8) (checkSum & 0xFF);

	// end flag.
	TX_BUF[msgLen++] = ZS_EFLAG;
	//xcps_TxEnabler_rs485();
	NRF24L01_Send();
	_delay_ms(20);
	NRF24L01_Send();
	_delay_ms(20);
	NRF24L01_Send();
	_delay_ms(20);

	//MSLEEP(1);
//	for (i = 0; i < msgLen; i++)
//		xcps_putter_rs485(buff[i]);
//	//xcps_TxDisabler_rs485();

	return 0;
}

/**

 */
int xcps_recv_rs485(uint8 *buff, int buff_length) {
	uint8 temp;
	int i;
	int checkSum = 0;

	// check getter.
	if (!xcps_getter_rs485)
		return -1;

	if (xcps_getter_rs485(&temp) < 1)
		return 0;

	switch (xcps_state_rs485) {
	case 0:
		xcps_rxlen_rs485 = 0;

		if (temp == ZS_SFLAG) {
			xcps_state_rs485 = 1;
			xcps_rx_packet_rs485[xcps_rxlen_rs485++] = temp;
		}

		else if (temp == ZS_EFLAG) {
			// don't change state, keep find start flag.
		}
		break;

	case 1: // found start flag
		xcps_rx_packet_rs485[xcps_rxlen_rs485++] = temp;
		xcps_state_rs485 = 2;
		xcps_pdu_len_rs485 = temp;

		break;

	case 2: // found length
		// fill data.
		xcps_rx_packet_rs485[xcps_rxlen_rs485++] = temp;

		// check length.
		if (xcps_rxlen_rs485 >= (2 + xcps_pdu_len_rs485))
			xcps_state_rs485 = 3;
		break;

	case 3: // data end, check checksum.
		for (i = 2; i < (2 + xcps_pdu_len_rs485); i++)
			checkSum += (xcps_rx_packet_rs485[i] & 0xFF);

		if (temp == (uint8) checkSum) // Checksum ok.
			xcps_state_rs485 = 4;

		else
			xcps_state_rs485 = 0;
		break;

	case 4:
		if (temp == ZS_EFLAG) {
			xcps_state_rs485 = 0;

			// return data to caller.
			for (i = 0; i < xcps_pdu_len_rs485; i++)
				buff[i] = xcps_rx_packet_rs485[2 + i];

			return xcps_pdu_len_rs485;
		} else {
			// TODO:
			xcps_state_rs485 = 0;
		}
		break;

	default:
		// if you here, something wrong. --> recover to state 0.
		xcps_state_rs485 = 0;
		break;
	}
	return 0;
}

int xcps_getOriginalCode(uint8 *resultBuff, uint8 *buff, int buff_length) {
	unsigned char resultLen = 0;
	unsigned char i = 0;
	for (i = 0; i < buff_length; i++) {
		if (buff[i] == 0xFF) {
			if (buff[i + 1] == 0xFF)
				resultBuff[resultLen++] = 0xFF;
			else if (buff[i + 1] == 0x01)
				resultBuff[resultLen++] = 0xFA;
			else if (buff[i + 1] == 0x02)
				resultBuff[resultLen++] = 0xAF;
			i++;
		} else
			resultBuff[resultLen++] = buff[i];
	}
	return resultLen;
}
int xcps_getTrnalatedCode(uint8 *resultBuff, uint8 *buff, int buff_length) {
	unsigned char resultLen = 0;
	unsigned char i = 0;
	for (i = 0; i < buff_length; i++) {
		if (buff[i] == 0xFA) {
			resultBuff[resultLen++] = 0xFF;
			resultBuff[resultLen++] = 0x01;
		} else if (buff[i] == 0xAF) {
			resultBuff[resultLen++] = 0xFF;
			resultBuff[resultLen++] = 0x02;
		} else if (buff[i] == 0xFF) {
			resultBuff[resultLen++] = 0xFF;
			resultBuff[resultLen++] = 0xFF;
		} else
			resultBuff[resultLen++] = buff[i];
	}
	return resultLen;
}

//end of file

