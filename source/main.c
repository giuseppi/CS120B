#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "io.c"
#include "dht.h"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
/*
void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
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
*/
void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int ADC_Read(char channel)
{
    int ADC_value;
    
    ADMUX = (0x40) | (channel & 0x07);/* set input channel to read */
    ADCSRA |= (1<<ADSC);    /* start conversion */
    while((ADCSRA &(1<<ADIF))== 0);    /* monitor end of conversion interrupt flag */
    
    ADCSRA |= (1<<ADIF);    /* clear interrupt flag */
    ADC_value = (int)ADCL;    /* read lower byte */
    ADC_value = ADC_value + (int)ADCH*256;/* read higher 2 bits, Multiply with weightage */

    return ADC_value;        /* return digital value */
}

int main(void)
{
    DDRA = 0x00; PORTA = 0x04;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    
    char buffer[40];
    int ADC_Value1;
    int ADC_Value2;
    
    ADC_init();        /* Initialize ADC */
    LCD_init();        /* Initialize LCD */
    LCD_ClearScreen();
    TimerSet(100);
    TimerOn();
    
    while(1)
    {
        unsigned click = ~PINA & 0x04;
        ADC_Value1 = ADC_Read(0);/* Read the status on X-OUT pin using channel 0 */
        ADC_Value2 = ADC_Read(1);/* Read the status on Y-OUT pin using channel 0 */
        
        if (click) {
            sprintf(buffer, "X=%d   Y=%d    Button pressed ", ADC_Value1, ADC_Value2);
        }
        else {
            sprintf(buffer, "X=%d   Y=%d    Button release", ADC_Value1, ADC_Value2);
        }
        
        LCD_DisplayString(1, buffer);
        LCD_Cursor(24);
        while (!TimerFlag) {};    // Wait 300ms
        TimerFlag = 0;
    }
    
}
