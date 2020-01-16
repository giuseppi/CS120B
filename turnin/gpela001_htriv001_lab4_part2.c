/*      Author: Giuseppi Pelayo - gpela001
 *       *       *  Partner(s) Name: Het Trivedi
 *        *        *      Lab Section: 022
 *         *         *      Assignment: Lab #4  Exercise #1
 *          *          *      Exercise Description: [optional - include for your own benefit]
 *           *           *
 *            *            *      I acknowledge all content contained herein, excluding template or example
 *             *             *      code, is my own original work.
 *              *              */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <RIMS.h>
#endif

enum part2_states{start} state;

void part2_SM() {
	
}

int main(void) {
        DDRA = 0x00;
        PORTA = 0xFF;
        DDRB = 0xFF;
        PORTB = 0x00;
        state = start;

        while(1) {
                part2_SM();
        }

    return 0;
}

