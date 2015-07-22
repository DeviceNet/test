#include "MDT-100Z.h"
#include "Rotary.h"
#include "NCProtocol.h"
#include "MD_Protocol.h"
#include "GC_Protocol.h"
#include "NRF24L01/NRF24L01.h"
#include "Debug.h"

// Avrx hw Peripheral initialization
#define CPUCLK 		16000000L     		// CPU xtal
#define TICKRATE 	1000				// AvrX timer queue Tick rate
#define TCNT0_INIT 	(CPUCLK/128/TICKRATE)

// NB: Clock divisor bits are different between classic and mega103 chips!
// NB: IAR chooses to use different CPU identifiers for their C compiler
// NB: GCC changes the IO bit names just for fun...
extern uint8_t RX_BUF[];
extern uint8_t TX_BUF[];
extern uint8_t RF_CHANNEL;
#define Open_TX
//#define Open_RX

#define TCCR0_INIT	((1<<CS02) | (0<<CS01) | (1<<CS00))
extern uint8 lastSendDimmingLevel[4];
uint8 SensorPacket[64];

//-----------------------------------------------------------------------------
//MCU Initialize
void InitMCU() {
	PORTA = 0x00;
	DDRA = 0xFF;
	PORTC = 0x0F;
	DDRC = 0xFF;
	PORTD = 0x9F;
	DDRD = 0xFF;
	PORTF = 0xFF;
	DDRF = 0xFF;
	PORTG = 0xFF;
	DDRG = 0xFF;
	DDRE |= _BV(PE0);
	DDRE &= ~_BV(PC4) | _BV(PC5) | _BV(PC6) | _BV(PC7);
	PORTE |= _BV(PC4) | _BV(PC5) | _BV(PC6) | _BV(PC7);

}

//----------------------------------------------------------------------//
void resetZigbee() {
	PORTD &= ~(0x80);
	MSLEEP(100);
	PORTD |= 0x80;
}

/***** Task Led(Toggle) *****/
void ledTask() {
	static uint8 mode = 0;
	if (timer_isfired(ON_WTD_TIMER_ID)) {
		wdt_reset();

		MDP_NRFSendDimmingReqToMDP(1, 1);
		timer_clear(ON_WTD_TIMER_ID);
		timer_set(ON_WTD_TIMER_ID, 100);
	}

	if (timer_isfired(ON_TEST_TIMER_ID)) {
		if (rotary_GetValue() == 0x00) {
			if (mode == 0) {
				mode++;
				lastSendDimmingLevel[0] = 0x34;
				lastSendDimmingLevel[1] = 0x34;
				lastSendDimmingLevel[2] = 0x34;
				lastSendDimmingLevel[3] = 0x34;
				MDP_SendDimmingReqToMDP(1, 1);
//				MDP_TESTSendDimmingReqToMDP(0xff, 0x33);
			} else if (mode == 1) {
				mode++;
				lastSendDimmingLevel[0] = 0x67;
				lastSendDimmingLevel[1] = 0x67;
				lastSendDimmingLevel[2] = 0x67;
				lastSendDimmingLevel[3] = 0x67;
				MDP_SendDimmingReqToMDP(1, 1);
//				MDP_TESTSendDimmingReqToMDP(0xff, 0x66);
			} else if (mode == 2) {
				mode++;
				lastSendDimmingLevel[0] = 0x9A;
				lastSendDimmingLevel[1] = 0x9A;
				lastSendDimmingLevel[2] = 0x9A;
				lastSendDimmingLevel[3] = 0x9A;
				MDP_SendDimmingReqToMDP(1, 1);
//				MDP_TESTSendDimmingReqToMDP(0xff, 0x99);
			} else if (mode == 3) {
				mode++;
				lastSendDimmingLevel[0] = 0xCD;
				lastSendDimmingLevel[1] = 0xCD;
				lastSendDimmingLevel[2] = 0xCD;
				lastSendDimmingLevel[3] = 0xCD;
				MDP_SendDimmingReqToMDP(1, 1);
//				MDP_TESTSendDimmingReqToMDP(0xff, 0xcc);
			} else if (mode == 4) {
				mode = 0;
				lastSendDimmingLevel[0] = 0xFE;
				lastSendDimmingLevel[1] = 0xFE;
				lastSendDimmingLevel[2] = 0xFE;
				lastSendDimmingLevel[3] = 0xFE;
				MDP_SendDimmingReqToMDP(1, 1);
//				MDP_TESTSendDimmingReqToMDP(0xff, 0xFE);
			} else {
				mode = 0;
			}
//			MDP_SendSetWatchdogReqToMDP(0);
		} else {
			MDP_SendDimmingReqToMDP(1, 1);

		}

		timer_clear(ON_TEST_TIMER_ID);
		timer_set(ON_TEST_TIMER_ID, 3000);
	}
}

void ZigbeeUsartTask() {
	static int Recvlen = 0;
	static uint8 zigbeeHWResetCount = 0;

	if (timer_isfired(ON_ZIGBEE_PING_TIMER_ID)) {
		ZRMsendPing();
		zigbeeHWResetCount++;

		timer_clear(ON_ZIGBEE_PING_TIMER_ID);
		timer_set(ON_ZIGBEE_PING_TIMER_ID, 10000);
	}

	if (getState() > 1) {
		zigbeeHWResetCount = 0;
	} else if (zigbeeHWResetCount > 5) {
		resetZigbee();
		zigbeeHWResetCount = 0;
	}

	if ((Recvlen = xcps_recv_zigbee(SensorPacket, 64)) > 0) {
		uint8 buff_len = 0;
		uint16 Src_Addr;
		uint16 Dst_Addr;
		uint16 destId = 0;

		switch (SensorPacket[0]) {
		case 0x00: // Send to GMProtocols (Gateway <-> RFM)
			buff_len = (Recvlen - 1);
			ZRMPMessage(&SensorPacket[1], buff_len);
			break;
		case 0x10:
			Dst_Addr = (uint16) (SensorPacket[1] << 8) | (SensorPacket[2]);
			Src_Addr = (uint16) (SensorPacket[3] << 8) | (SensorPacket[4]);

			destId = (uint16) (SensorPacket[9] << 8) | (SensorPacket[10]);
			buff_len = (Recvlen - 5);
			if (destId == tmp_zrmp.zrm_Id) {
				timer_clear(NCP_REREGISTER_ID);
				timer_set(NCP_REREGISTER_ID, 2000);

				XNetHandler(Src_Addr, &SensorPacket[5], buff_len - 5);
			}
			break;

		default: // Error Mesaage
			break;
		}
	}

	if (timer_isfired(NCP_REREGISTER_ID)) {
		ncp_SendRegisterReq();

		timer_clear(NCP_REREGISTER_ID);
		timer_set(NCP_REREGISTER_ID, 2000);
	}
}

void NCP_Task() {
	if (timer_isfired(NCP_TIMER_ID)) {
		if (ncp_ConnState() == NCP_NET_NOT_CONNECT) {
			if (getState() == ZRM_CONNECT)
				ncp_SendRegisterReq();

			timer_clear(NCP_TIMER_ID);
			timer_set(NCP_TIMER_ID, 10000);
		} else {
			if (getState() == ZRM_CONNECT)
				ncp_SendPingReq();

			timer_clear(NCP_TIMER_ID);
			timer_set(NCP_TIMER_ID, 20000);
		}
	}
}

void WDT_INIT() {
	MCUCSR &= ~(1 << WDRF); // WatchDog Init(Low)
	wdt_enable(WDTO_2S);
	// WatchDog Reset Time(High)
}

//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
int main(void) {

	WDT_INIT();
	InitMCU();
	initRotary();
	InitUART();
	timer_init();
	rotary__GetValue();
	nRF24L01_Initial();

	// initialize dmx driver
	MDP_SendDimmingReqToMDP(0xff, 0xFF);

	//DMX512Send();
	PORTC = 0x00;

	TCCR1A = 0x00;
	TCCR1B = 0x06;
	TCNT1H = 0xFF;
	TCNT1L = 0xFF;

	TIMSK |= 0x04;

	ZRMSendSetPreconfig(0x15);
	MSLEEP(100);
	ZRMSendReset();
	MSLEEP(1000);

	timer_set(ON_WTD_TIMER_ID, 500);
	timer_set(ON_TEST_TIMER_ID, 3000);

	timer_set(ON_ZIGBEE_PING_TIMER_ID, 3000);
	timer_set(ON_ONTIME_TIMER_ID, 100);
	timer_set(NCP_TIMER_ID, 2000);

	tmp_zrmp.zrm_State = ZRM_INIT;

	xcps_init_zigbee(USART0_Receive, USART0_Transmit);
	xcps_init_rs485(USART1_Receive, USART1_Transmit);
//	FILE mystdout =
	TX_Mode(0);
//			FDEV_SETUP_STREAM((void *)USART0_Transmit, NULL, _FDEV_SETUP_WRITE);
//	stdout = &mystdout;
//
//	DEBUG("\n\r=========START PROGRAM ===========\n\r");
	while (1) {
		ledTask();
		ZigbeeUsartTask();
		NCP_Task();
	}

	return 0;
}

