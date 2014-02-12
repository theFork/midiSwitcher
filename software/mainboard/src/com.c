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
 * 2-wire communication protocol for the midi switcher
*/

#include "com.h"

uint16_t receive_com( int8_t bits )
{
    uint16_t data = 0;
    for (bits--; bits >= 0; bits--)
    {
        // read data bit
        data |= COM_DATA<<bits;
        while ( COM_ACTIVE );

        if ( bits == 0 ) // last bit received
        {
            return data;
        }

        // wait for next bit
        while ( !COM_ACTIVE );
    }
    return data; // never reached
}

void transmit_com( uint16_t data , int8_t bits )
{
    for (bits--; bits >= 0; bits--)
    {
        // set / clear data bit
        if ( data>>bits & 0x1 )
            PORT_DAT_OUT |= BIT_DAT_OUT;
        else
            PORT_DAT_OUT &=~ BIT_DAT_OUT;

        // transmit
        PORT_CLK_OUT |= BIT_CLK_OUT;
        _delay_us(500);
        PORT_CLK_OUT &=~ BIT_CLK_OUT;
        _delay_us(500);
    }
}
