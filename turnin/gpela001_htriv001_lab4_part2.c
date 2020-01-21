#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum part2_states{start, init, plus, minus, reset} state;

void part2_SM() {
	unsigned char val;
	switch(state) { // Transitions
		case start:
			state = init;
			val = 0x07;
			break;
		case init:
			if (PORTA == 0x03) {state = reset;}
			else if (PORTA == 0x02) {state = minus;}
			else if (PORTA == 0x01) {state = plus;}
			else {state = init;}
			break;
		case minus:
			if (PORTA == 0x00) {state = init;}
			else if (PORTA == 0x03) {state = reset;}
			else {state = minus;}
			break;
		case plus:
			if (PORTA == 0x00) {state = init;}
			else if (PORTA == 0x03) {state = reset;}
			else {state = plus;}
			break;
		case reset:
			if (PORTA == 0x03) {state = reset;}
				else {state = init;}
			break;
		default:
			break;
		}
	
	switch (state) { // State Actions			
		case init:
			break;
		case minus:
			if (val > 0) {val--;}
			break;
		case plus:
			if (val < 9) {val++;}
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
