/**
	PLCS 1.0

	@author	hjchoi@m2mkorea.co.kr
	@date 2011/06/01
*/
/*
	Timer utility.

	NOTE: Only 8MHz clock system is tested. Other clock will produce diff result.
*/

#ifndef	__TIMER_H__
#define	__TIMER_H__


#include "Util.h"


/**
	Define number of timers to use.
*/
#define MAX_SYS_TIMER		10

#define timer_isfired(x)	(!(timer_get(x)))


#define DISABLE_INTERRUPT()     asm("cli")
#define ENABLE_INTERRUPT()      asm("sei")


void timer_init();

/*
	time value in milliseconds.
	- about 4000000.000 seconds will be counted.
*/
int timer_set(int timer_id, uint32 time_value);
int timer_get(int timer_id);
int timer_clear(int timer_id);

#endif

