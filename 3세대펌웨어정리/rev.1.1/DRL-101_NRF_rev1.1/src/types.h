#ifndef	__TYPES_H__
#define	__TYPES_H__
/**
	basic type definitions
*/



typedef	unsigned char 	uint8;
typedef unsigned short 	uint16;
typedef unsigned long	uint32;

#ifdef BUILD_LINUX_ENV
	typedef unsigned long long	TICK_COUNT;
#else
	typedef uint32		TICK_COUNT;
#endif


// use string...
//#include <string>

#endif	//__TYPES_H__
