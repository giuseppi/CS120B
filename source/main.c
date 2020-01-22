#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum part3_states{start, wait, push_1, r_1, open} state;
static unsigned char val;

void part3_SM() {
	switch(state) {
		case start:
			state = wait;
			break;
		case wait:
			if (PINA == 0x04) {state = push_1;}
			else {state = wait;}
			break;
		case push_1:
			if (PINA == 0x00) {state = r_1;}
			else {state = push_1;}
			break;
		case r_1:
			if (PINA == 128) {state = wait;}
			else if (PINA == 0x02) {state = open;}
			else if (PINA == 0x00) {state = r_1;}
			else {state = wait;}
			break;
		case open:
			if (PINA == 128) {state = wait;}
			else if (PINA = 0x00) {state = open;}
			else {state = wait;}
			break;
		default:
			state = start;
			break;
		}
	
	switch (state) {
		case start:
			break;
		case wait:
			val = 0x00;
			break;			
		case open:
			val = 0x01;
			break;
		case r_1:
			break;
		case push_1:
			break;
		default:
			val = 0x00;
			break;
	}
}

int main(void) {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	PORTC = 0x00;
	state = start;
    
	while (1) {
		part3_SM();
		PORTB = val;
	}
	return 0;
}
