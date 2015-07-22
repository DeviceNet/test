/**
	atmega128 conf.

	2010/02/02. for EIS_V10, dmx dimmer config
	m2mkorea Inc.
	sungyun,
	
*/


//-----------------------------------------------------------------------------
//	
	// dmx512 driver
	#define UBRR1
	#define	UART_SEL	1	
	//#undef UBRR0H	
	#define	DMX512SRAMSIZE	32	// defined, DMX512.Data[] array

	// dmx binary type (dmxID band selecting)
	// 0 : default
	// 1 : 
	// 2 :
	#define	DMX_BIN_TYPE	0

	#if (DMX_BIN_TYPE)

		#if (DMX_BIN_TYPE == 1)
			#define	MAX_DMX_ID	31	// 0x1F
		#elif (DMX_BIN_TYPE == 2)
			#define	MAX_DMX_ID	47	// 0x2F
		#else
		#endif
		
	#else
		#define	MAX_DMX_ID		15	// 0x0F
	#endif


// end of conf.
