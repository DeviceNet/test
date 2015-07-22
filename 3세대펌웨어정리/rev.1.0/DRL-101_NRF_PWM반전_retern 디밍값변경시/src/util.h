#ifndef __UTIL_H__
#define __UTIL_H__
/**
	Utility for porting thread facility.
	- mutex
	- thread
	- sleeping.

	2009/02/01. modified to handle sim/porting.
*/

#ifdef	WIN32

#include	<windows.h>

#define		LOG				printf
#define		LOG_DEBUG		

#define		SSLEEP(x)		Sleep((x)*1000)
#define		MSLEEP(x)		Sleep(x)
#define		USLEEP			Sleep

#elif defined (__GNUC__)
	// winavr gcc
#include	<stdio.h>
#include	"util/delay.h"
#define		LOG(...)

#define		SSLEEP(x)		_delay_ms((x)*1000)
#define		MSLEEP(x)		_delay_ms(x)
#define		USLEEP(x)		_delay_us(x)

#define WATCHDOG_ENABLE_VALUE		100
#define WATCHDOG_RESET_COUNT_VALUE		200

#else
	// CVAVR

#include	<stdio.h>

#define		LOG(...)

#define		SSLEEP(x)		delay_ms((x)*1000)
#define		MSLEEP(x)		delay_ms(x)
#define		USLEEP			Sleep

#endif

#endif
//__UTIL_H__

