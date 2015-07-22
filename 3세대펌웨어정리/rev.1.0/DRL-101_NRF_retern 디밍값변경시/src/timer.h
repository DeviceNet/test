#ifndef	__TIMER_H__
#define	__TIMER_H__
/*
	Timer utility
	
	- timer interrupt initialization
	- timer application utility.
		timer_set();
		timer_get();
		
	; Use poolling to check timer fired event, cause no thread feature.
	@usage
		// init timer interrupt routine.
		timer_init();
		
		timer_set(0, time_millis);
		
		if (timer_fired(0))
		{
			do_something();
			
			// if to use again...
			// if not reset timer, next timer_fired() will return true again.
			timer_set(0, time_millis);
		
			// if timer is not used anymore.	
			timer_clear();
		}
	
	@note
		System clock timer를 사용하므로 오차가 심하니, 
			1분에 1초이상 오차가 있음을 가정할것.

	2009/07/24. timer util startup.
*/
#include "types.h"


/**
	Define number of timers to use.
*/
#define		MAX_SYS_TIMER	8

#define	timer_isfired(x)	(!(timer_get(x)))

void timer_init();

/*
	time value in milliseconds.
	- about 4000000.000 seconds will be counted.
*/
int timer_set(int timer_id, uint32 time_value);
int timer_get(int timer_id);
int timer_clear(int timer_id);

#endif
