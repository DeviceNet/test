#ifndef	__UART_H_
#define	__UART_H_

/**
	ATmega128 USART Interface.
**/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "Util.h"

#define DISABLE_INTERRUPT()     asm("cli")
#define ENABLE_INTERRUPT()      asm("sei")


/**
	command writer (via uart) setting

	int uart_putchar0(uint8 *c);
*/

typedef void (*usart_TxEnabler)();
typedef void (*usart_TxDisabler)();

typedef	int (*usart_getter)(uint8 *data);
typedef	int (*usart_putter)(uint8 data);


/**

*/

void InitUART();

void USART0_TxEnable();
void USART0_TxDisable();

void USART1_TxEnable();
void USART1_TxDisable();

int USART0_Transmit(uint8 buff);
int USART0_Receive(uint8 *buff);

int USART1_Transmit(uint8 buff);
int USART1_Receive(uint8 *buff);


#endif
