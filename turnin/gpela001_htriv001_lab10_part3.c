#include <avr/io.h>
#include <avr/interrupt.h>	

enum Three_States{T_Start, B0, B1, B2} three_state;
enum Blink_States{B_Start, off, B3} blink_state;
enum Speaker_States{S_Start, Quiet, Noise}speak_state;
enum Output{Start, write} out_state;
	
unsigned char tmp_three = 0x00;
unsigned char tmp_blink = 0x00;
unsigned char tmp_A = 0x00;
	
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

void set_PWM(double frequency) {

	static double current_frequency;

	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954)
      {
         OCR3A = 0xFFFF;
      }
		else if (frequency > 31250) {
          OCR3A = 0x0000;
       }
		else {
         OCR3A = (short)(8000000 / (128 * frequency)) - 1;
       }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
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
		case B_Start: blink_state = B3; break;
		case off: blink_state = B3; break;
		case B3: blink_state = off; break;
		default: break;
	}
	switch (blink_state) {
		case B_Start: break;
		case off: tmp_blink = 0x00; break;
		case B3: tmp_blink = 0x10; break;
		default: break;
	}
}

void speaker_Tick() {
	switch (speak_state) {
		case Start: speak_state = Quiet; break;
		case Quiet: speak_state = (tmp_A == 0x04) ? Noise : Quiet; break;
		case Noise: speak_state = Quiet; break;
		default: break;
	}
	switch (speak_state) {
		case Start: break;
		case Quiet: set_PWM(0); break;
		case Noise: set_PWM(261.62); break;
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned long TL_elapsedTime = 0;
	unsigned long BL_elapsedTime = 0;
	unsigned long S_elapsedTime = 0;
	const unsigned long timerPeriod = 2;
	PWM_on();
    TimerSet(timerPeriod);
	TimerOn();
	three_state = T_Start;
	blink_state = B_Start;
	speak_state = S_Start;
    while (1) 
    {
		tmp_A = ~PINA & 0x04;
		if (TL_elapsedTime >= 300) {
			threeLED_Tick();
			TL_elapsedTime = 0;
		}
		if (BL_elapsedTime >= 1000) {
			blinkLED_Tick();
			BL_elapsedTime = 0;
		}
		if (S_elapsedTime >= 2) {
			speaker_Tick();
			S_elapsedTime = 0;
		}
		output_Tick();
		
		while (!TimerFlag) {}
		TimerFlag = 0;
		
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
		S_elapsedTime += timerPeriod;
    }
}
