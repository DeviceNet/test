#include "rotary.h"
#include <avr/io.h>
#include <avr/iom128.h>

void initRotary() {
	DDRB &= ~(0xF0);
	PORTB |= 0xF0;
}

uint8 rotary_GetValue() {

	uint8 id = 0;

	if ((PINB & _BV(7)) == 0) {
		id |= _BV(3);
	}
	if ((PINB & _BV(6)) == 0) {
		id |= _BV(2);
	}
	if ((PINB & _BV(5)) == 0) {
		id |= _BV(1);
	}
	if ((PINB & _BV(4)) == 0) {
		id |= _BV(0);
	}
	return id;
}
