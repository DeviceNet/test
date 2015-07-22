#ifndef __UTIL_H__
#define __UTIL_H__

#include	<stdio.h>
#include	"util/delay.h"


//#define DEBUG_ENABLE

#define		MSLEEP(x)		_delay_ms(x)
#define		USLEEP(x)		_delay_us(x)


//------------------------------------------------------------------------//
//-------------------------- Timer ID -------------------------------------//
//------------------------------------------------------------------------//
#define ON_ZIGBEE_PING_TIMER_ID		1
#define ON_WTD_TIMER_ID			2
#define ON_DIMMING_CONTROL_TIMER_ID	3
#define ON_ONTIME_TIMER_ID	4

#define ON_TEST_TIMER_ID	5
#define NCP_TIMER_ID 6

#define NCP_REREGISTER_ID 7






//------------------------------------------------------------------------//
//-------------------------- Timer Count -----------------------------------//
//------------------------------------------------------------------------//
#define TIME_COUNT				1			// per 1min's


#define PING_TIME					60


typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;


#endif

