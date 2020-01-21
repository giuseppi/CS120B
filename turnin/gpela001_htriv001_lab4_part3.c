#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum part3_states{start, wait, push_1, r_1, push_Y, open} state;

void part3_SM() {
	switch(state) {
		case start:
			state = wait;
			break;
		case wait:
			PORTB = 0x00;
			if (PORTA == 0x04) {state = push_1;}
			else {state = wait;}
			break;
		case push_1:
			if (PORTA == 0x00) {state = r_1;}
			else {state = push_1;}
			break;
		case r_1:
			if (PORTA == 0x02) {state = push_Y;}
			else if (PORTA == 0x00) {state = r_1;}
			else {state = wait;}
			break;
		case push_Y:
			state = wait;
			break;
		default:
			break;
		}
	
	switch (state) {			
		case push_Y:
			PORTB = 0x01;
			break;
		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	state = start;
    
	while (1) {
		part3();
	}
	return 0;
}
