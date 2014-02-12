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
* midi I/O header
*/

#ifndef _MIDI_H
#define _MIDI_H


//---------------- constants ----------------//
// MIDI baudrate
#define     BAUD                31250

// maximum MIDI value
#define     MIDI_MAX_VALUE      127

// MIDI status byte
#define     MIDI_COMMAND_MASK   0xf0
#define     MIDI_PROGRAM_CHANGE 0xc0
#define     MIDI_CONTROL_CHANGE 0xb0
#define     MIDI_NOTE_ON        0x90

// MIDI data byte 0 for NOTE ON commands
#define     CMD_PITCH_SHIFTER   0x01
#define     CMD_TAP_TEMPO       0x02


//---------------- data types ----------------//
// codes for MIDI state machine
typedef enum {
    IDLE,
    NOTE_ON,
    CONTROL_CHANGE,
    PROGRAM_CHANGE,
} midi_state_t;


//---------------- functions and procedures ----------------//
void configureUSART( void );


//---------------- EOF ----------------//
#endif // _MIDI_H

