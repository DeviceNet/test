#include "Uart.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#define USART_BUFF_SIZE	64
//------------------------------------------------------//
uint8 uart_rx_buff[USART_BUFF_SIZE];
uint8 uart_rx_front = 0;
uint8 uart_rx_tail = 0;
uint8 uart_rx_len = 0; //------------------------------------------------------//

void InitUART(void) {
//	UCSRA = 0x00;
//	UCSRB = 0x90;
//	UCSRC = 0x86;
//	UBRRH = 0x00;
//	UBRRL = 0x33; //9600

	UCSR0A = 0x00; // U2X = 0
	UCSR0B = 0x98;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 0x33; // 9600
}

//--------------------- USART0 -------------------------//
//------------------------------------------------------//
uint8 status;
uint8 data;
ISR(USART_RX_vect) {
	DISABLE_INTERRUPT();
	while (((status = UCSR0A) & (1 << RXC0)) == 0)
		;

	data = UDR0;

	uart_rx_buff[uart_rx_tail] = data;
	uart_rx_tail = (uart_rx_tail + 1) % USART_BUFF_SIZE;
	uart_rx_len++;
	ENABLE_INTERRUPT();
}
ISR(INT0_vect){

}
//------------------------------------------------------//

//------------------------------------------------------//
int USART_Receive(uint8 *buff) {

	DISABLE_INTERRUPT();

	if (uart_rx_len > 0) {
		uart_rx_len--;
		ENABLE_INTERRUPT();
		*buff = uart_rx_buff[uart_rx_front];
		uart_rx_front = (uart_rx_front + 1) % USART_BUFF_SIZE;

		return 1;
	}

	ENABLE_INTERRUPT();

	return 0;
}

int USART_Transmit(uint8 buff) {
	while ((UCSR0A & (1 << UDRE0)) == 0)
		;
	// tx isr.

	UDR0 = buff;
	return 1;
}

// End of file

