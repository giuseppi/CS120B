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
#include <RIMS.h>
#endif

enum part1_states{start, 01_press, 01_release, 10_press, 10_release} state;

void part1_SM() {
  switch(state) { //Transitions
    case start:
      state = 01_press;
      break;
    case 01_press:
      if (A0) {
        state = 01_press;
      }
      else if (!A0) {
        state = 01_release;
      }
      else {
	state = 01_press;
      }
      break;
    case 01_release:
      if (!A0) {
        state = 01_release;
      }
      else if (A0) {
        state = 10_press;
      }
      else {
	state = 01_release;
      }
      break;
    case 10_press:
      if (A0) {
	state = 10_press;
      else if (!A0) {
        state = 10_release;
      }
      else {
	state = 10_press;
      }
      break;
    case 10_release:
      if (!A0) {
        state = 10_release;
      }
      else if (A0) {
        state = 01_press;
      }
      else {
        state = 10_release;
      }
      break;
    default:
      state = start;
      break;
  }

  switch(state) { //State Actions
    case start:
      break;
    case 01_press:
      B0 = 0x01;
      B1 = 0x00;
      break;
    case 01_release:
      break;
    case 10_press:
      B0 = 0x00;
      B1 = 0x01;
      break;
    case 10_release:
      break;
    default:
      B0 = 0x01;
      B1 = 0x00;
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
		
    return 0;
}
