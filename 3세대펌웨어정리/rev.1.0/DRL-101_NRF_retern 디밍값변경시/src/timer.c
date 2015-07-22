#include "avr/io.h"
#include "avr/interrupt.h"
#include "timer.h"

#define		MAX_SYS_TIMER	8

typedef struct _sys_timer {
	uint8 running;
	uint32 value;
} sys_timer;

sys_timer timer_list[MAX_SYS_TIMER];

int timer_set(int timer_id, uint32 time_value) {

	if ((timer_id >= 0) && (timer_id < MAX_SYS_TIMER)) {
		//	asm("cli");
		timer_list[timer_id].value = time_value;
		timer_list[timer_id].running = 1;
		//	asm("sei");
	}
	return 0;
}

int timer_get(int timer_id) {
	if ((timer_id >= 0) && (timer_id < MAX_SYS_TIMER)) {
		if (timer_list[timer_id].running) {
			if (timer_list[timer_id].value == 0)
				return 0;
			else
				return 1;
		} else
			return 1;
	} else {
		// return as not fired.
		return 1;
	}
}

int timer_clear(int timer_id) {
	if ((timer_id >= 0) && (timer_id < MAX_SYS_TIMER)) {
		timer_list[timer_id].running = 0;
	}
	return 0;
}

/**
 initialize timer.

 use TIM0.
 */
void timer_init() {
	for (int i = 0; i < MAX_SYS_TIMER; i++) {
		timer_list[i].running = 0;
		timer_list[i].value = 0;
	}

	// check _M_CPU.. system clk.

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 7.813 kHz
	// Mode: Normal top=FFh
	// OC0 output: Disconnected

	// TIMER_1MS	// atmega8.
	// overflow interrupt setting. 1ms interval tick.
	TCCR0B = 0x05;

//    TCNT0=0x69;
	TCNT0 = 0xF8;

	TIMSK0 = 0x01;
	// 0x02: OCIE0 - timer0 compare match interrupt enable.
}

ISR(TIMER0_OVF_vect) {
	int i;
	// reset watchdog timer.
//	_WDR();

	// 1ms timer.

	// timer decreasing.
	for (i = 0; i < MAX_SYS_TIMER; i++) {
		if (timer_list[i].value > 0) {
			timer_list[i].value--;
		}
	}

	// reset timer init.
	TCNT0 = 0xF8; // init value; only OVF int.
}
