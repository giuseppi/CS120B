#include <avr/io.h>
#include <avr/interrupt.h>	

enum Three_States{T_Start, B0, B1, B2} three_state;
enum Blink_States{B_Start, off, B3} blink_state;
enum Output{Start, write} out_state;
	
unsigned char tmp_three = 0x00;
unsigned char tmp_blink = 0x00;
	
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
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
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
	
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
 
void threeLED_Tick() {
	switch (three_state) {
		case T_Start: three_state = B0; break;
		case B0: three_state = B1; break;
		case B1: three_state = B2; break;
		case B2: three_state = B0; break;
		default: break;
	}
	switch (three_state) {
		case T_Start: break;
		case B0: tmp_three = 0x01; break;
		case B1: tmp_three = 0x02; break;
		case B2: tmp_three = 0x04; break;
		default: break;
	}
}

void blinkLED_Tick() {
	switch (blink_state) {
		case B_Start: blink_state = off; break;
		case off: blink_state = B3; break;
		case B3: blink_state = off; break;
		default: break;
	}
	switch (blink_state) {
		case B_Start: break;
		case off: tmp_blink = 0x00; break;
		case B3: tmp_blink = 0x08; break;
		default: break;
	}
}

void output_Tick() {
	switch (out_state) {
		case Start: out_state = write; break;
		case write: break;
		default: break;
	}
	switch (out_state) {
		case Start: break;
		case write: 
			PORTB = tmp_blink | tmp_three; 
			break;
		default: break;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	unsigned long TL_elapsedTime = 0;
	unsigned long BL_elapsedTime = 0;
	const unsigned long timerPeriod = 100;
    TimerSet(100);
	TimerOn();
	three_state = Start;
	blink_state = B_Start;
    while (1) 
    {
		if (TL_elapsedTime >= 300) {
			threeLED_Tick();
			TL_elapsedTime = 0;
		}
		if (BL_elapsedTime >= 1000) {
			blinkLED_Tick();
			BL_elapsedTime = 0;
		}
		output_Tick();
		
		while (!TimerFlag) {}
		TimerFlag = 0;
		
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
    }
}

