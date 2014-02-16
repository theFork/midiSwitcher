/*
 * Copyright 2012-2014 Sebastian Neuser
 *
 * This file is part of the MIDI volume controller firmware.
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
 * along with the MIDI volume controller firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * midi I/O procedures
*/

#include "com.h"
#include "midi.h"
#include "midiSwitcher.h"
#include "program.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/setbaud.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

midi_state_t    midi_state;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configureUSART( void )
{
    // enable RXEN and RXC interrupt
    UCSRB = _BV(RXC) | _BV(RXEN);

    // apply UBRR value computed by setbaud.h
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
#if USE_2X
    UCSRA |= _BV(U2X);
#else
    UCSRA &=~ _BV(U2X);
#endif
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(USART_RXC_vect)
{
    // disable interrupts
    cli();

    // wait until transmission is complete
    while ( !(UCSRA & 1<<RXC) );

    // fetch data
    uint8_t data = UDR;

    switch (midi_state) {
        case IDLE:
            if ( (data & MIDI_COMMAND_MASK) == MIDI_NOTE_OFF )
                midi_state = NOTE_OFF;
            else if ( (data & MIDI_COMMAND_MASK) == MIDI_NOTE_ON )
                midi_state = NOTE_ON;
            else if ( (data & MIDI_COMMAND_MASK) == MIDI_CONTROL_CHANGE )
                midi_state = CONTROL_CHANGE;
            else if ( (data & MIDI_COMMAND_MASK) == MIDI_PROGRAM_CHANGE )
                midi_state = PROGRAM_CHANGE;
            break;

        case NOTE_OFF:
        case NOTE_ON:
            if (data == CMD_MOMENTARY) {
                // toggle switch0
                toggleChannel(MOMENTARY_SWITCH_CHANNEL);
            }
            midi_state = IDLE;
            break;

        case PROGRAM_CHANGE:
            if ( data < PROGRAM_COUNT ) {
                enterProgram( data );
                transmit_com( PROG_CHANGE<<7 | data , 10 );
            }
            midi_state = IDLE;
            break;

        default:
            midi_state = IDLE;
            break;
    }

    // enable interrupts
    sei();
}
