/*
 * Debug.h
 *
 *  Created on: 2012. 7. 25.
 *      Author: GoldSunda
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#define DEBUG(FORMAT,args...) 			PORTE = 0x0F;printf_P(PSTR(FORMAT),##args)
//#define DEBUG(FORMAT,args...) 			printf_P(PSTR(FORMAT),##args)
#define DEBUG(...) 	;
#define SPRINTF(target,FORMAT,args...) 	sprintf_P(target,PSTR(FORMAT),##args)
#define STRNCMP(target,FORMAT,args...) 	strncmp_P(target,PSTR(FORMAT),##args)

#endif /* DEBUG_H_ */
