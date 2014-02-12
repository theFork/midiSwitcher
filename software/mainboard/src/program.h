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
 * header for the program module
*/

#ifndef _PROGRAM
#define _PROGRAM


//---------------- includes ----------------//
#include <avr/eeprom.h>
#include <util/delay.h>


//---------------- constants ----------------//
#define     PROGRAM_COUNT       120


//---------------- data types ----------------//
// program data
typedef union {
    uint16_t word;
    struct {
        uint16_t    looper5     : 1;
        uint16_t    looper4     : 1;
        uint16_t    looper6     : 1;
        uint16_t    looper7     : 1;
        uint16_t    impulse1    : 1;
        uint16_t    impulse0    : 1;
        uint16_t    switch1     : 1;
        uint16_t    switch0     : 1;
        uint16_t    looper2     : 1;
        uint16_t    looper3     : 1;
        uint16_t    looper1     : 1;
        uint16_t    looper0     : 1;
    } channels;
} program_data_t;

// program
typedef struct
{
    uint8_t         number;
    program_data_t  data;
} program_t;


//---------------- functions and procedures ----------------//
void applyProgramData( program_data_t program );
void copyCurrentBankTo( uint8_t pgm );
void copyCurrentProgramTo( uint8_t pgm );
void enterProgram( uint8_t );
uint16_t readProgram( uint8_t );
void toggleChannel(uint8_t number);
void updateProgram( uint8_t number, uint16_t config );
void wipeCurrentProgram( void );
void wipeCurrentBank( void );


//---------------- EOF ----------------//
#endif // _PROGRAM
