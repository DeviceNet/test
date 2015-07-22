#include "MD_Protocol.h"
#include "xcps.h"
#include "nc_protocol.h"
#include <avr/eeprom.h>
#include "Debug.h"
uint8 isWatchdogEnabled = 255;
uint8 watchdogResetCount = 255;

uint8 PWM_change(uint8 data) {
	uint8 out_data = 0;

	for (int i = 0; i < 254; i++) {
		if (data == i) {
			out_data = 255 - i;
		}
	}
	return out_data;
}

void MDP_ProcessMessage(uint8 msg[], int length) {
	uint8 msgType;

	msgType = msg[8];

	switch (msgType) { // Msg Type of NCP
	case PLCS_DP_REQ_DIMMING:
		MDP_ProcessDimmingReq(msg, length);
		break;
	case PLCS_DP_REQ_WATCHDOG:
		MDP_ProcessSetWatchdogReq(msg, length);
		break;
	default:
		break;
	}
}

void MDP_ProcessDimmingReq(uint8 msg[], int length) {
	uint8 channelId = 0;
	uint8 dimmingLevel = 0;

	channelId = msg[11];
	dimmingLevel = msg[12];
	if (msg[11] == 0x00) {
//		g_DimmingLevel[0] = msg[12];
		g_DimmingLevel[0] = PWM_change(msg[12]);
//		DEBUG("channel 0 : 0x%02X\n\r", msg[12]);

	}
	if (msg[13] == 0x01) {
//		g_DimmingLevel[1] = msg[14];
		g_DimmingLevel[1] = PWM_change(msg[14]);
//		DEBUG("channel 1 : 0x%02X\n\r", msg[14]);
	}
	if (msg[15] == 0x02) {
//		g_DimmingLevel[2] = msg[16];
		g_DimmingLevel[2] = PWM_change(msg[16]);
//		DEBUG("channel 2 : 0x%02X\n\r", msg[16]);
	}
	if (msg[17] == 0x03) {
//		g_DimmingLevel[3] = msg[18];
		g_DimmingLevel[3] = PWM_change(msg[18]);
//		DEBUG("channel 3 : 0x%02X\n\r", msg[18]);
	}

	g_TestFlag = 1;

	PORTD ^= 0x40;

	if (MDP_GetWatchdogResetCount() > 0) {
		MDP_SetWatchdogResetCount(0);
	}
}

void MDP_ProcessSetWatchdogReq(uint8 msg[], int length) {
	isWatchdogEnabled = msg[11];
	if (eeprom_read_byte((uint8_t*) WATCHDOG_ENABLE_VALUE) != isWatchdogEnabled)
		eeprom_write_byte((uint8_t*) WATCHDOG_ENABLE_VALUE, isWatchdogEnabled);
}

uint8 MDP_IsWatchdogEnable() {
	if (isWatchdogEnabled == 255) {
		isWatchdogEnabled = eeprom_read_byte((uint8_t*) WATCHDOG_ENABLE_VALUE);
	}
	return isWatchdogEnabled;
}

uint8 MDP_GetWatchdogResetCount() {
	if (watchdogResetCount == 255) {
		watchdogResetCount = eeprom_read_byte(
				(uint8_t*) WATCHDOG_RESET_COUNT_VALUE);
	}
	return watchdogResetCount;
}

uint8 MDP_SetWatchdogResetCount(uint8 value) {
	if (watchdogResetCount != value) {
		watchdogResetCount = value;
		eeprom_write_byte((uint8_t*) WATCHDOG_RESET_COUNT_VALUE, value);
	}
	return 0;
}
