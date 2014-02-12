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
 * header for the COM protocol
*/

#ifndef COM
#define COM


//---------------- includes ----------------//
#include <stdint.h>


//---------------- AVR PORT mapping ----------------//
#define        PORT_CLK_OUT    PORTD
#define        PORT_CLK_IN    PIND
#define        PORT_DAT_OUT    PORTD
#define        PORT_DAT_IN    PIND


//---------------- AVR bit names ----------------//
#define        BIT_CLK_OUT    0x80
#define        BIT_DAT_OUT    0x20
#define        BIT_CLK_IN    0x08
#define        BIT_DAT_IN    0x02


//---------------- code macros ----------------//
#define        COM_ACTIVE    ( PORT_CLK_IN & BIT_CLK_IN )
#define        COM_DATA    ( (PORT_DAT_IN & BIT_DAT_IN) / BIT_DAT_IN )


//---------------- functions and procedures ----------------//
extern uint16_t receive_com( int8_t bits );
extern void transmit_com( uint16_t data , int8_t bits );


//---------------- EOF ----------------//
#endif // COM
