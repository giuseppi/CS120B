#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

	enum STATES { START, INIT, INCREMENT, RESET, DECREMENT } state;
	unsigned char holder;

void tick() {
	switch(state) {
		case START:
			state = INIT;
			holder = 7;
			break;
		case INIT:
			if (PINA == 3) {
				state = RESET;
			} else if (PINA == 1) {
				state = INCREMENT;
			} else if (PINA == 2) {
				state = DECREMENT;
			} else {
				state = INIT;
			}
			break;
		case INCREMENT:
			state = INIT;
			break;
		case RESET:
			state = INIT;
			break;
		case DECREMENT:
			state = INIT;
			break;
	}

	switch(state) {
		case START:
			break;
		case INIT:
			break;
		case INCREMENT:
			if (holder < 9) {
			holder++;
			}
			break;
		case RESET:
			holder = 0;
			break;
		case DECREMENT:
			if (holder > 0) {
			holder--;
			}
			break;
	}
}

int main(void) {
DDRA = 0x00; DDRC = 0xFF; PORTA = 0xFF; PORTC = 0x00;
state = START;
holder = 7;
    while (1) {
	tick();
	PORTC = holder;
    }
    return 1;
}
