#ifndef	__UART_H_
#define	__UART_H_

/**
	ATmega128 USART Interface.
**/


#include "types.h"

#define DISABLE_INTERRUPT()     asm("cli")
#define ENABLE_INTERRUPT()      asm("sei")


/**
	command writer (via uart) setting

	int uart_putchar0(uint8 *c);
*/

typedef	int (*usart_getter)(uint8 *data);
typedef	int (*usart_putter)(uint8 data);


void InitUART();
int USART_Transmit(uint8 buff);
int USART_Receive(uint8 *buff);


#endif
