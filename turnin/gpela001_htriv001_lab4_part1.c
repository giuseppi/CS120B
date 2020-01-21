/*      Author: gpela001
 *       *  Partner(s) Name: Het Trivedi
 *        *      Lab Section: 022
 *         *      Assignment: Lab #4  Exercise #1
 *          *      Exercise Description: [optional - include for your own benefit]
 *           *
 *            *      I acknowledge all content contained herein, excluding template or example
 *             *      code, is my own original work.
 *              */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum part1_states{start, p_01, r_01, p_10, r_10} state;

void part1_SM() {
  unsigned char A0 = PINA; 
  switch(state) { //Transitions
    case start:
      state = p_01;
      break;
    case p_01:
      if (A0) {
        state = p_01;
      }
      else if (!A0) {
        state = r_01;
      }
      else {
	state = p_01;
      }
      break;
    case r_01:
      if (!A0) {
        state = r_01;
      }
      else if (A0) {
        state = p_10;
      }
      else {
	state = r_01;
      }
      break;
    case p_10:
      if (A0) {
	state = p_10;
      else if (!A0) {
        state = r_10;
      }
      else {
	state = p_10;
      }
      break;
    case r_10:
      if (!A0) {
        state = r_10;
      }
      else if (A0) {
        state = p_01;
      }
      else {
        state = r_10;
      }
      break;
    default:
      state = start;
      break;
  }

  switch(state) { //State Actions
    case start:
      break;
    case p_01:
      PORTB = 0x01;
      break;
    case r_01:
      break;
    case p_10:
      PORTB = 0x10;
      break;
    case r_10:
      break;
    default:
      PORTB = 0x01;
      break;
  }
}

int main(void) {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	state = start;

	while(1) {
		part1_SM();		
	}
		
    return 1;
}
