/**
	PLCS 1.0

	@author	hjchoi@m2mkorea.co.kr
	@date 2011/06/01
*/
/*
	Timer utility.

	NOTE: Only 16MHz clock system is tested. Other clock will produce diff result.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"


#ifndef	MAX_SYS_TIMER
#define	MAX_SYS_TIMER		10
#endif

/**
	TIMER0 is used by AvrX, so use timer3 for my timer.
*/
#define	USE_TIMER3

//-- @note : these are in MGW200Z.h
#define	CPUCLK		16000000L
#define	TIMER_TICK	1000										// 1ms interval, 1000 tick per seconds
#define 	TIMER_TCNT (CPUCLK/256/TIMER_TICK)
//-- @endnote

typedef	struct _sys_timer
{
	uint8	running;
	uint32	value;
} sys_timer;


sys_timer timer_list[MAX_SYS_TIMER];

// internal timer count..
uint32 timerCount;


int timer_set(int timer_id, uint32 time_value)
{
	
	if ( (timer_id >= 0) && (timer_id < MAX_SYS_TIMER) )
	{
		//DISABLE_INTERRUPT();
		
		timer_list[timer_id].value = time_value;
		timer_list[timer_id].running = 1;
		
		//ENABLE_INTERRUPT();
	}
	return 0;
}


int timer_get(int timer_id)
{
	if ( (timer_id >= 0) && (timer_id < MAX_SYS_TIMER) )
	{
		if (timer_list[timer_id].running)
		{
			if (timer_list[timer_id].value == 0)
				return 0;
			else
				return 1;
		}
		else
			return 1;
	}
	else
	{
		// return as not fired.
		return 1;
	}
}



int timer_clear(int timer_id)
{
	if ( (timer_id >= 0) && (timer_id < MAX_SYS_TIMER) )
	{
		timer_list[timer_id].running = 0;
	}
	
	return 0;
}


/**
	initialize timer.
	- timer utility init
	- timer register setting
*/
void timer_init()
{
	DISABLE_INTERRUPT();
	for (int i=0; i<MAX_SYS_TIMER; i++)
	{
		timer_list[i].running = 0;
		timer_list[i].value = 0;
	}
	
#ifdef	USE_TIMER3
	// 1ms.
	// TCCR0 : FOC0, WGM00, COM01, COM00, WGM01, CS02, CS01, CS00
	// CS12,CS11,CS10 : 1=clk, 2=clk/8, 3=clk/32, 4=clk/64, 5=clk/128, 6=clk/256, 7=clk/1024.
	// WGM01,WGM00 : 0x10=CTC(Clear Timer on Compare match) mode; top = OCR0, 
	// timer control : CLK_io = system clock = 8 MHz.

	TCNT3H = 0x00;
	TCNT3L = 0x00;

	TCCR3A = 0x40;				// Normal mode.  WGMn[3-0] = b0100.
								// COM1A1,A0 = b00, normal port operation.(CTC mode)
	TCCR3B = 0x0C;				// CSn2/1/0 = b100, clk_io/256, CTC mode.
	TCCR3C = 0x00;
	
	OCR3AH = 0x00;
	OCR3AL = TIMER_TCNT;		//value=31 for 8Mhz;		// 1ms

	ETIMSK |=0x10;				// enable OCIE3A, don't touch other flags.
	
#endif
 	ENABLE_INTERRUPT();

}




#ifdef	USE_TIMER3
ISR(TIMER3_COMPA_vect)
{ 
	int i;
	
	// Need not clear TCNT1, it is automatically cleared on CTC mode.(Clear Timer on Compare match)
	// TCNT1 = 0;
	// 1ms timer.
	timerCount++;
	
	// timer decreasing.
	for (i=0; i<MAX_SYS_TIMER; i++)
	{
		if (timer_list[i].value > 0)
		{
			timer_list[i].value--;
		}
	}

}
#endif	// USE_TIMER1


/*
	note : When using AvrX, timer0 is used as scheduler.
	so, TIMER1 is used instead
	
	
	TODO:
*/
