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
 * main program of the midi switcher
*/

#include "com.h"
#include "midi.h"
#include "midiSwitcher.h"
#include "program.h"

#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

// initialization and endless loop
int main( void )
{
    // configure GPIO ports
    configurePorts();

    // configure USART
    configureUSART();

    // fetch dummy bit to sync COM interface
    receive_com( 1 );

    // activate interrupts
    sei();

    // main program
    int8_t pressedSW;
    while ( 1 )
    {
        pressedSW = getButtonNumber();
        if ( pressedSW >= 0 && pressedSW <= 11 ) {
            toggleChannel(pressedSW);
        }

        if ( COM_ACTIVE )
        {
            // fetch package
            uint16_t data = receive_com( 10 );

            // extract command and program/bank number
            uint8_t cmd = data >> 7;
            uint8_t pgm = data & 0x7f;

            switch ( cmd )
            {
                case PROG_CHANGE:
                    enterProgram( pgm );
                    break;

                case COPY_PROGRAM:
                    copyCurrentProgramTo( pgm );
                    break;

                case COPY_BANK:
                    copyCurrentBankTo( pgm );
                    break;

                case WIPE_PROGRAM:
                    wipeCurrentProgram();
                    break;

                case WIPE_BANK:
                    wipeCurrentBank();
                    break;
            }
        }
    }

    return 0;
}
