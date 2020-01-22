#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum part2_states{start, init, plus, minus, reset} state;

void part2_SM() {
	unsigned static char val;
	unsigned char A0 = PINA & 0x01;
	unsigned char A1 = PINA;
	switch(state) { // Transitions
		case start:
			state = init;
			val = 0x07;
			break;
		case init:
			if (A0 && A1) {
				state = reset;
			}
			else if ((val >= 0) && (A1 && !A0)) {
				state = minus;
			}
			else if ((val < 9) && (A0 && !A1)) {
				state = plus;
			}
			else {
				state = init;
			}
			break;
		case minus:
			if (!A0 && !A1) {
				state = init;
			}
			else if (A0 && A1) {
				state = reset;
			}
			else {
				state = minus;
			}
			break;
		case plus:
			if (!A0 && !A1) {
				state = init;
			}
			else if (A0 && A1) {
				state = reset;
			}
			else {
				state = plus;
			}
			break;
		case reset:
			if (A0 && A1) {
				state = reset;
			}
			else {
				state = init;
			}
			break;
		default:
			break;
		}
	
	switch (state) { // State Actions			
		case init:
			break;
		case minus:
			val -= 0x01;
			break;
		case plus:
			val += 0x01;
			break;
		case reset:
			val = 0x00;
			break;
		default:
			break;
		
	}
	PORTC = val;
}

int main(void) {
    	DDRA = 0x00;
	PORTA = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	state = start;

    	while(1) {
		part2_SM();
	}
	return 0;
}
