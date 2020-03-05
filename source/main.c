#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110.h"

int main(void)
{
    DDRB = 0xFF; PORTB = 0x00;
    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_write_string("IT'S WORKING!",1);
    nokia_lcd_set_cursor(0, 10);
    nokia_lcd_write_string("Nice!", 3);
    nokia_lcd_render();

    while(1){
    }
    return 1;
}
