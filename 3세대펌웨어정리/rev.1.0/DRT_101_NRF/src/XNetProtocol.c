#include "XNetProtocol.h"
#include "NCProtocol.h"
#include "MD_Protocol.h"
#include "GC_Protocol.h"

// --------------------------------------------------------------------------- //
uint8 tmp_Buff[64];
// --------------------------------------------------------------------------- //

void XNetHandler(uint16 srcNetAddr, uint8 msg[], int buff_length) {
	uint8 pid;
	uint8 subPid;
	uint8 msgType;

	pid = msg[0];
	subPid = msg[1];
	switch (pid) {
	case NCP_PROTOCOL_ID: {
		switch (subPid) {
		case PLCS_NCP_PROTOCOL_ID:
			ncp_ProcessMessage(srcNetAddr, msg, buff_length);
			break;
		case PLCS_GCP_PROTOCOL_ID:
			plcs_GCP_ProcessMessageFromZigbee(srcNetAddr, msg, buff_length);
			break;
		case PLCS_DP_PROTOCOL_ID:
			plcs_MDP_ProcessMessageFromZigbee(srcNetAddr, msg, buff_length);
			break;
		}
	}
		break;
	default:
		break;
	}
}

//end of file

