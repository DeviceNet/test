#ifndef __MAIN_H__
#define __MAIN_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <avr/wdt.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Util.h"
#include "Uart.h"
#include "Xcps.h"
#include "ZRMProtocol.h"
#include "Timer.h"

// YELLOW LED(LOW ACTIVE) : LC-100Z State Check LED
#define STATE_LED_ON()		(PORTD &= ~0x10)			// Low
#define STATE_LED_OFF()		(PORTD |= 0x10)				// High


void InitMCU();
void Start_LED();
void WDT_INIT();

#endif

