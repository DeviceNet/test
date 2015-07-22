
#include "Uart.h"


#define USART_BUFF_SIZE	256
//------------------------------------------------------//
uint8 uart0_rx_buff[USART_BUFF_SIZE];
uint8 uart0_rx_front = 0;
uint8 uart0_rx_tail = 0;
uint8 uart0_rx_len = 0;

uint8 uart0_tx_buff[USART_BUFF_SIZE];
uint8 uart0_tx_front = 0;
uint8 uart0_tx_tail = 0;
uint8 uart0_tx_len = 0;
//------------------------------------------------------//

//------------------------------------------------------//
uint8 uart1_rx_buff[USART_BUFF_SIZE];
uint8 uart1_rx_front = 0;
uint8 uart1_rx_tail = 0;
uint8 uart1_rx_len = 0;

uint8 uart1_tx_buff[USART_BUFF_SIZE];
uint8 uart1_tx_front = 0;
uint8 uart1_tx_tail = 0;
uint8 uart1_tx_len = 0;
//------------------------------------------------------//


#ifdef DEBUG_ENABLE
//***************************************************************************//
	static int Putchar(char c, FILE *stream);

	static FILE mystdout = FDEV_SETUP_STREAM(Putchar, NULL, _FDEV_SETUP_RW);

	static int Putchar(char c, FILE *stream)
	{
		if (c == '\n')
		Putchar('\r', stream);
		loop_until_bit_is_set(UCSR1A, UDRE1);
		UDR1 = c;
		return 0;
	}
//***************************************************************************//
#endif

void InitUART(void)
{
	UCSR0A = 0x02;		// U2X = 0
	UCSR0B = 0xD8;
	UCSR0C = 0x06;		  
	UBRR0H = 0x00;
	UBRR0L = 0x10;		// 115200 bps, double speed


	UCSR1A=0x00;
	UCSR1B=0x08;
	UCSR1C=0x06;
	UBRR1H=0x00;
	UBRR1L=0x67; //9600

#ifdef DEBUG_ENABLE
	stdout = &mystdout;
#endif
}

/*
void USART1_TxEnable(){
	UCSR1B = 0xD8;
}
void USART1_TxDisable(){
	UCSR1B = 0x90;
}*/


//--------------------- USART0 -------------------------//
//------------------------------------------------------//
ISR(USART0_RX_vect)
{
	uint8 status;
	uint8 data;

    while (((status = UCSR0A) & (1 << RXC)) == 0);

	data = UDR0;

	uart0_rx_buff[uart0_rx_tail] = data;
	uart0_rx_tail = (uart0_rx_tail+1) % USART_BUFF_SIZE;
	uart0_rx_len ++;
}


ISR(USART0_UDRE_vect)
{
	uint8 buff;

 	if (uart0_tx_len > 0)
	{
		buff = uart0_tx_buff[uart0_tx_front];

	    	UDR0 = buff;

		uart0_tx_front = (uart0_tx_front+1) % USART_BUFF_SIZE;
		uart0_tx_len--;
	}

    UCSR0B &= ~0x20;    // RXCIE, UDRIE, RXEN, TXEN
}


ISR(USART0_TX_vect)
{
	uint8 buff;

	if (uart0_tx_len > 0)
	{
		buff = uart0_tx_buff[uart0_tx_front];
		//while ((UCSR1A & (1<<UDRE))==0);
		// tx isr.

		UDR0 = buff;

		uart0_tx_front = (uart0_tx_front+1) % USART_BUFF_SIZE;
		uart0_tx_len--;
	}
}
//------------------------------------------------------//

//------------------------------------------------------//
int USART0_Receive(uint8 *buff)
{
	
	DISABLE_INTERRUPT();
 
	if (uart0_rx_len > 0)
	{		
		uart0_rx_len--;
    	ENABLE_INTERRUPT();
		*buff = uart0_rx_buff[uart0_rx_front]; 		
		uart0_rx_front = (uart0_rx_front+1) % USART_BUFF_SIZE;

		return 1;
	}

   	ENABLE_INTERRUPT();

	return 0;
}


int USART0_Transmit(uint8 buff)
{

	while ((UCSR0A & (1<<UDRE))==0);
		// tx isr.

	UDR0 = buff;
	return 1;
/*	
	DISABLE_INTERRUPT();
    	UCSR0B |= 0x20;    // UDRIE set : enable DRE interrupt

	if (uart0_tx_len < USART_BUFF_SIZE)
	{
		uart0_tx_buff[uart0_tx_tail] = buff;
		uart0_tx_tail = (uart0_tx_tail+1) % USART_BUFF_SIZE;
		uart0_tx_len ++;

    		ENABLE_INTERRUPT();

		return 1;
	}

   	ENABLE_INTERRUPT();
*/	
	return 0;
}
//------------------------------------------------------//


//--------------------- USART1 --------------------------//
//------------------------------------------------------//

ISR(USART1_RX_vect)
{
	uint8 status;
	uint8 data;

    	while (((status = UCSR1A) & (1 << RXC))==0);

	data = UDR1;

	uart1_rx_buff[uart1_rx_tail] = data;
	uart1_rx_tail = (uart1_rx_tail+1) % USART_BUFF_SIZE;
	uart1_rx_len ++;
}


ISR(USART1_UDRE_vect)
{
	uint8 buff;

 	if (uart1_tx_len > 0)
	{
		buff = uart1_tx_buff[uart1_tx_front];

	    	UDR1 = buff;

		uart1_tx_front = (uart1_tx_front+1) % USART_BUFF_SIZE;
		uart1_tx_len--;
	}

    UCSR1B &= ~0x20;    // RXCIE, UDRIE, RXEN, TXEN
}


ISR(USART1_TX_vect)
{
	uint8 buff;

	if (uart1_tx_len > 0)
	{
		buff = uart1_tx_buff[uart1_tx_front];
		//while ((UCSR1A & (1<<UDRE))==0);
		// tx isr.
	   	 UDR1 = buff;
		uart1_tx_front = (uart1_tx_front+1) % USART_BUFF_SIZE;
		uart1_tx_len--;
	}
}
//------------------------------------------------------//

int USART1_Receive(uint8 *buff)
{
	DISABLE_INTERRUPT();
	
	if (uart1_rx_len > 0)
	{
		uart1_rx_len--;
    	ENABLE_INTERRUPT();
		*buff = uart1_rx_buff[uart1_rx_front];
		uart1_rx_front = (uart1_rx_front+1) % USART_BUFF_SIZE;

		return 1;
	}

   	ENABLE_INTERRUPT();

	return 0;
}


int USART1_Transmit(uint8 buff)
{
	while ((UCSR1A & (1<<UDRE))==0);
		// tx isr.
   	 UDR1 = buff;
	return 1;
}

// End of file

