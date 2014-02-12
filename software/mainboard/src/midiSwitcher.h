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
 * header for the MIDI switcher
*/

#ifndef _MIDI_SWITCHER
#define _MIDI_SWITCHER


//---------------- includes ----------------//
#include <avr/interrupt.h>
#include <com.h>


//---------------- PORT mapping ----------------//
#define        COM_B3           7
#define        COM_B2           5
#define        COM_B1           3
#define        COM_B0           1


//---------------- COM commands ----------------//
#define        DISPLAY_MSG      0x02
#define        PROG_CHANGE      0x03
#define        COPY_PROGRAM     0x04
#define        COPY_BANK        0x05
#define        WIPE_PROGRAM     0x06
#define        WIPE_BANK        0x07


//---------------- data types ----------------//
// execution state
typedef struct
{
    uint8_t progChange;
} exec_state_t;


//---------------- functions and procedures ----------------//
// interface.c
void configPorts( void );
void configUSART( void );
int8_t getButtonNumber( void );


//---------------- EOF ----------------//
#endif // _MIDI_SWITCHER
