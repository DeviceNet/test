#include <stdio.h>
#include <string.h>
#include "Xcps.h"
#include "Uart.h"
#include <avr/io.h>

// protocol message format start/end flag.
#define	ZS_SFLAG	0xFA
#define	ZS_EFLAG	0xAF

/**
 temporary rx packet.
 */
uint8 xcps_rx_packet[XCPS_MAX_PDU];

int xcps_rxlen = 0; // total length received.
int xcps_pdu_len = 0;

static int xcps_state = 0;

/**
 uart interface to use.
 */
static usart_getter xcps_getter = (usart_getter) 0;
static usart_putter xcps_putter = (usart_putter) 0;

/**

 **/
int xcps_init(usart_getter getter, usart_putter putter) {
	xcps_getter = getter;
	xcps_putter = putter;

	return 0;
}

int xcps_recv(uint8 *buff, int buff_length) {
	uint8 temp;
	int i;
	int checkSum = 0;

	// check getter.
	if (!xcps_getter)
		return -1;

	if (xcps_getter(&temp) < 1)
		return 0;

	if (xcps_rxlen >= XCPS_MAX_PDU)
		xcps_state = 0;

	switch (xcps_state) {
	case 0:
		xcps_rxlen = 0;

		if (temp == ZS_SFLAG) {
			xcps_state = 1;
			xcps_rx_packet[xcps_rxlen++] = temp;
		}

		else if (temp == ZS_EFLAG) {
			// don't change state, keep find start flag.
		}
		break;

	case 1: // found start flag
		xcps_rx_packet[xcps_rxlen++] = temp;
		xcps_state = 2;
		xcps_pdu_len = temp;

		break;

	case 2: // found length
		// fill data.
		xcps_rx_packet[xcps_rxlen++] = temp;

		// check length.
		if (xcps_rxlen >= (2 + xcps_pdu_len))
			xcps_state = 3;

		break;

	case 3: // data end, check checksum.
		for (i = 2; i < (2 + xcps_pdu_len); i++)
			checkSum += (xcps_rx_packet[i] & 0xFF);

		if (temp == (uint8) checkSum) // Checksum ok.
			xcps_state = 4;

		else
			xcps_state = 0;
		break;

	case 4:
		if (temp == ZS_EFLAG) {
			xcps_state = 0;

			// return data to caller.
			for (i = 0; i < xcps_pdu_len; i++)
				buff[i] = xcps_rx_packet[2 + i];

			return xcps_pdu_len;
		} else {
			// TODO:
			xcps_state = 0;
		}
		break;

	default:
		// if you here, something wrong. --> recover to state 0.
		xcps_state = 0;
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

