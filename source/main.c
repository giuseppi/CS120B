#include <avr/io.h>

enum States{Start, Idle, C, D, E}state;
unsigned char tmpA = 0x00;

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

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

void Tick() {
	switch (state) {
		case Start:
			state = Idle;
			break;
		case Idle:
			if (tmpA == 0x04) {
				state = C;
			} 
			else if (tmpA == 0x02) {
				state = D;
			}
			else if (tmpA == 0x01) {
				state = E;
			}
			else {
				state = Idle;
			}
			break;
		case C:
			state = (tmpA == 0x04) ? C : Idle;
			break;
		case D:
			state = (tmpA == 0x02) ? D : Idle;
			break;
		case E:
			state = (tmpA == 0x01) ? E : Idle;
			break;
		default: break;
	}
	switch (state) {
		case Start: break;
		case Idle:
			set_PWM(0); 
			break;
		case C:
			set_PWM(261.63);
			break;
		case D:
			set_PWM(293.66);
			break;
		case E:
			set_PWM(329.63);
			break;
		default:
			break;
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	PWM_on();

    while (1) {
		tmpA = ~PINA & 0x07;
		Tick();
    }
}

