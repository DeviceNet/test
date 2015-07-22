#include "XNetProtocol.h"
#include <string.h>
#include "NC_Protocol.h"
#include "Xcps.h"
#include "MD_Protocol.h"
#include "Debug.h"
uint8 XNetHandler(uint8 buff[], int buff_length) {
	uint8 pid;
	uint8 subPid;

	pid = buff[0];
	subPid = buff[1];
//	DEBUG("BUFF = ");
//
//	for (int i = 0; i < 22; i++) {
//		DEBUG("%02X ", buff[i]);
//
//	}
//	DEBUG("\n\r");

	switch (pid) {
	case NCP_PROTOCOL_ID: {
		switch (subPid) {
		case PLCS_DP_PROTOCOL_ID:
			MDP_ProcessMessage(buff, buff_length);
			break;
		}
	}
		break;
	default:
		break;
	}

	return 1;
}

//end of file

