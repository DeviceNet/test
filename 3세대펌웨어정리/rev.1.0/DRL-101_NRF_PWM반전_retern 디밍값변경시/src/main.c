//-----------------------------------------------------------------------------
//	refference
//
#include "avr/interrupt.h"
#include "avr/io.h"
#include <stdlib.h>	// rand
#include "conf.h"		// dmx dimmer conf.
#include "types.h"
#include "util.h"
#include "timer.h"
#include "xnetProtocol.h"
#include "xcps.h"
#include <avr/eeprom.h>
#include "MD_Protocol.h"
#include "stdio.h"
#include <avr/wdt.h>
#include "Debug.h"
#include "NRF24L01/NRF24L01.h"
#define ENABLE		1
#define DISABLE		0

#define ON			1
#define OFF			0
#define Open_RX
#define IRQ		  (PIND & (1<<2))

#define CE_0      PORTD &= ~(1<<6);
#define CE_1      PORTD |= (1<<6);

#define CSN_0     PORTD &= ~(1<<7);
#define CSN_1     PORTD |= (1<<7);
extern uint8_t RX_BUF[];
extern uint8_t TX_BUF[];
extern uint8_t RF_CHANNEL;
//LED Driver shutdown
#define SHUT_DOWN(x)		do{\
								if(x==DISABLE) PORTC |= 0x01;\
								else	   PORTC &= ~0x01;\
							   }while(0)
//PWM
#define PWM_OUT(x)			do{OCR1AL = x;}while(0)

//-----------------------------------------------------------------------------
//	definition
//

//-----------------------------------------------------------------------------
//	functions
//
//void _WDR() {
//	int i = 0;
//
//	WDTCSR = 0x18;
//	WDTCSR = 0x00;
//	for (i = 0; i < 5; i++)
//		;
//
//	WDTCSR = 0x18;
//	WDTCSR = 0x0F;
//	for (i = 0; i < 5; i++)
//		;
//}
void rotary__GetValue() {
	//id
	DDRC &= ~_BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5);
	PORTC |= _BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5);

	_delay_ms(1);

	uint8 id = 0;

	if ((PINC & _BV(5)) == 0) {
		id |= _BV(3);
	}
	if ((PINC & _BV(4)) == 0) {
		id |= _BV(2);
	}
	if ((PINC & _BV(3)) == 0) {
		id |= _BV(1);
	}
	if ((PINC & _BV(2)) == 0) {
		id |= _BV(0);
	}

	if (id == 0) {
		RF_CHANNEL = 94;
	} else {
		RF_CHANNEL = 94 + (2 * id);

	}

}
void at8_init(void);

void delay(long t) {
	while (t--)
		;
}

//	wdt_enable(WDTO_2S);
//	wdt_reset();

//	wdt_disable();

uint8 g_DimmingLevel[4];
uint8 g_TestFlag = 0;
uint8 TestFlag = 221;

int main() {
	wdt_enable(WDTO_2S);
	wdt_reset();
	//
	uint8 DIPC = 0;
	uint16 dmxID = 0;
	uint8 jiffies = 0;
	uint8 value = 0;
	uint8 fadeOutCnt = 0;
	uint8 Recvlen = 0;
	uint8 UsartPacket[64];

	wdt_reset();
	MSLEEP(1000);
	wdt_reset();

	//
//	_WDR();
	//
	at8_init();

	// init system timer. (using TIM0)
	timer_init();

	asm("sei");
	// Global enable interrupts

//	_WDR();
	wdt_reset();

// initialize dmx driver
	InitUART();
	xcps_init(USART_Receive, USART_Transmit);
	nRF24L01_Initial();
	rotary__GetValue();
	RX_Mode();
// initialize dmx buffer, dmxID = 0
	g_DimmingLevel[0] = g_DimmingLevel[1] = g_DimmingLevel[2] =
			g_DimmingLevel[3] = 0x00;

	// Shutdown is toggled for stabilization of led driver chipset
	timer_set(0, 100);

	//fadeIn, fadeOut Dimming control timer
	timer_set(1, 50);

	//ChannelId reading timer: Rotary Switch.
	timer_set(2, 1000);
	timer_set(3, 1000); // 1sec
	timer_set(5, 1000); // 1sec
//	FILE mystdout =
//			FDEV_SETUP_STREAM((void *)USART_Transmit, NULL, _FDEV_SETUP_WRITE);
//	stdout = &mystdout;
//
//	DEBUG("\n\r=========START PROGRAM ===========\n\r");

	while (1) {
		SHUT_DOWN(ENABLE);
		SHUT_DOWN(DISABLE);
		if (timer_isfired(0)) {
			break;
		}
	}
	SHUT_DOWN(DISABLE);
	PWM_OUT(0xFF);

	while (1) {
		if (IRQ == 0) {
			timer_set(6, 60000);

			NRF24L01_Receive();
		}
		if ((Recvlen = xcps_recv(UsartPacket, 64)) > 0) {
			timer_set(6, 60000);

			XNetHandler(UsartPacket, Recvlen);
		}
		if (timer_isfired(3)) {
			wdt_reset();
			PORTD ^= 0x20;
			dmxID = 0;
			if ((PIND & _BV(3)) == 0) {
				dmxID += 8;
			}
			if ((PIND & _BV(4)) == 0) {
				dmxID += 4;
			}
			if ((PINC & _BV(0)) == 0) {
				dmxID += 2;
			}
			if ((PINC & _BV(1)) == 0) {
				dmxID += 1;
			}
			if (dmxID >= 4)
				dmxID = 3;

			if (timer_isfired(6)) {
				RX_Mode();
				g_DimmingLevel[0] = g_DimmingLevel[1] = g_DimmingLevel[2] =
						g_DimmingLevel[3] = 0x00;
				timer_clear(6);
			}
			timer_set(3, 1000); // 1sec
		}

		// dimming level control : PWM
		if (OCR1AL != g_DimmingLevel[dmxID]) {
			if (OCR1AL == 0x00) {
				SHUT_DOWN(ENABLE);
				jiffies = 1;
			} else {
				if (jiffies == 1) {
					jiffies = 0;

					// Shutdown is toggled for stabilization of led driver chipset
					timer_set(0, 100);
					while (1) {
						SHUT_DOWN(ENABLE);
						SHUT_DOWN(DISABLE);
						if (timer_isfired(0))
							break;
					}
					SHUT_DOWN(DISABLE);
					jiffies = 0;
				}
			}

			if (timer_isfired(1)) {
				if (g_DimmingLevel[dmxID] > OCR1AL) {
					value = (g_DimmingLevel[dmxID] - OCR1AL) / 10;
					if (value < 5)
						value = 1;
					PWM_OUT(OCR1AL+value);
				} else {
					value = (OCR1AL - g_DimmingLevel[dmxID]) / 10;
					if (value > 5)
						value = 3;
					else
						value = 1;

					if (fadeOutCnt < 5)
						value = 0;
					else
						fadeOutCnt = 0;

					fadeOutCnt++;
					PWM_OUT(OCR1AL-value);
				}
				timer_clear(1);
				timer_set(1, 1);
			}
		}

	}

	return 0;
}

//-----------------------------------------------------------------------------
//
void at8_init(void) {
	//-------------------------------
//	PORTB = 0x3F;
//	DDRB = 0x00;
//	PORTC = 0x00;
//	DDRC = 0x00;
	PORTB = 0x3F;
	DDRB = 0x00;
	DDRC = 0x00;
	PORTC |= _BV(PC0) | _BV(PC1);
	DDRD &= ~_BV(PD3) | _BV(PD4);
	PORTD |= _BV(PD3) | _BV(PD4);
	//-------------------------------
	// dmx dimmer, initialization
	//
	// led driver and pwm
	// 	: PB1(A_PWM)-O, PB2(B_PWM)-O, PC1(A_GPIO)-O, PC0(B_GPIO)-O
	// dip switch
	// 	: PD5(8Digit)-?, PD6(4Digit)-?, PD7(2Digit)-I, PB0(1Digit)-I
	// dmx512
	// 	: PD0(UART)-I,
	//
//	PORTB = 0x02;
//	DDRB = 0x02;
//	PORTC = 0x1E;
//	DDRC &= ~(0x1E);
//
//	DDRD = 0x60;
	PORTB = 0x02;
	DDRB = 0x02;
	//	PORTC = 0x1E;
	//	DDRC &= ~(0x1E);

	DDRD |= _BV(PD6) | _BV(PD5);
	//-------------------------------
	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 15.625 kHz
	TCCR0B = 0x05;
	TCNT0 = 0xF0;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 16000.000 kHz
	// Mode: Ph. correct PWM top=01FFh
	// OC1A output: Non-Inv.
	// OC1B output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer 1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A = 0xA1; // 0xA2: 9 bit. top=0x01FF., 0xA1: 8 bit. top=0x00FF.
	TCCR1B = 0x03; // no prescaling, 2=1/8 prescale, 3=1/64 prescale, ...
	TCNT1H = 0x00; // initial counting value
	TCNT1L = 0x00;
	ICR1H = 0x00;
	ICR1L = 0x00; //0x00;
	OCR1AH = 0x00;
	OCR1AL = 0x00; //0x00; (A-port) dimming control value. (initial value)
	OCR1BH = 0x00;
	OCR1BL = 0x00; //0x00; (B-port) dimming control value. (initial value)

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: Timer 2 Stopped
	// Mode: Normal top=FFh
	// OC2 output: Disconnected
	ASSR = 0x00;
	TCCR2B = 0x00;
	TCNT2 = 0x00;
	OCR2A = 0x00;

	// External Interrupt(s) initialization
	// INT0: Off
	// INT1: Off
	MCUCR = 0x00;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK0 = 0x01;
}

// end of main.
