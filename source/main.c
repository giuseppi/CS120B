#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States{start, init, NextLed} state;

void tick() {
	switch(state) { // Transitions
		case start:
			state = init;
		case init:
			state = NextLed;
		case NextLed:
			state = NextLed;
	}
	switch(state) { // State Actions
		case start:
			break;
		case init:
			PORTB = 0x01;
		case NextLed:
			if (PORTB == 0x80) {PORTB = 0x01;}
			else {PORTB = PORTB << 1;}
		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();
	unsigned char tmpB = 0x00;
    
	while (1) {
		tick();
		tmpB = ~tmpB;
		PORTB = tmpB;
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
