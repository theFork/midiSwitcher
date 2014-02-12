/*
 * Copyright 2012-2014 Sebastian Neuser
 *
 * This file is part of the MIDI switcher firmware.
 *
 * The MIDI volume controller firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The MIDI volume controller firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the MIDI switcher firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * software / hardware interface of the midi switcher
*/

#include "gpio.h"

#include <avr/io.h>
#include <util/delay.h>



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configurePorts( void )
{
    // switch and relay/LED ports
    DDRA = 0x0f;    // [7:4] in; [3:0] out
    PORTA = 0xf0;    // activate pull-up resistors for inputs
    DDRB = 0x0f;    // [7:4] in; [3:0] out
    PORTB = 0xf0;    // activate pull-up resistors for inputs
    DDRC = 0x0f;    // [7:4] in; [3:0] out
    PORTC = 0xf0;    // activate pull-up resistors for inputs
    DDRD = 0xa0;    // [7,5] COM out, [3,1] COM in ; [0] rxd
    PORTD = 0x00;    // deactivate all pull-up resistors
}

int8_t getButtonNumber( void )
{
    // input vector
    int vector = ACTIVEPINS;

    if (vector)
    {
        // compute the button number
        int result = 0;
        while (vector % 2 == 0)
        {
            vector >>= 1;
            result++;
        }

        // multiple switches pressed -> error
        vector--;
        if(vector != 0) return -1;

        // de-bounce
        _delay_ms(100);
        while (ACTIVEPINS);

        return result;
    }
    return -1;
}

void setOutputs(uint16_t data)
{
    PORTA = 0xf0 | (data & 0x00f)>>0;
    PORTB = 0xf0 | (data & 0x0f0)>>4;
    PORTC = 0xf0 | (data & 0xf00)>>8;
}
