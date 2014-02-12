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
 * header for the gpio interface
*/

#ifndef _GPIO
#define _GPIO


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants and macros ----------------//
// USART
#define BAUDRATE 31250
#define UBRRHVAL (uint8_t)((F_OSC/BAUDRATE/16-1)>>8)
#define UBRRLVAL (uint8_t)((F_OSC/BAUDRATE/16-1))

// buttons
#define ACTIVEPINS ((~PINC & 0xf0)<<4) | (~PINB & 0xf0) | ((~PINA & 0xf0)>>4)


//---------------- functions and procedures ----------------//
// interface.c
void configurePorts( void );
int8_t getButtonNumber( void );
void setOutputs( uint16_t data );


//---------------- EOF ----------------//
#endif // _GPIO
