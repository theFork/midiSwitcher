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

#include "midiSwitcher.h"
#include "program.h"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

extern program_t current_program;

// main state variable struct
exec_state_t    state;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

// initialization and endless loop
int main( void )
{
    // configure in-/out- ports
    configPorts();

    // configure in-/out- ports
    configUSART();

    // initialize variables
    state.progChange = 0;

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

// INTERRUPT-FUNCTIONS
// midi package received
ISR(USART_RXC_vect)
{
    // wait until transmission is complete
    while ( !(UCSRA & 1<<RXC) );

    // fetch data
    uint8_t data = UDR;

    if ( state.progChange == 1 ) // PC flag set
    {
        state.progChange = 0;
        if ( data < 120 )
        {
            enterProgram( data );
            transmit_com( PROG_CHANGE<<7 | data , 10 );
        }
    }
    else if ( (data & 0xF0) == 0xC0 ) // in case of PC, set flag
        state.progChange = 1;
}
