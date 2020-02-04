#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE); 
}

int main(void) {
	DDRA = 0x00; DDRB = 0xFF; DDRD = 0xFF;
	PORTA = 0xFF; PORTB = 0x00; PORTD = 0x00;
	unsigned short adcVal;
	unsigned char bval;
	unsigned char dval;
	ADC_init();
	
	while(1) {
		adcVal = ADC;
		bval = (char)adcVal;
		dval = (char)(adcVal >> 4);
		if (adcVal >= 0xFF / 2) {
			PORTB = bval;
			PORTD = dval;
		}
		else {
			PORTB = 0;
			PORTD = 0;		
		}
	}
}
