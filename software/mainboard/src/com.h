/*
 * header for the COM protocol
 * @author haggl
 * @version 0.1.0 (1): 2011-09-02
*/

#ifndef COM
#define COM


//---------------- includes ----------------//
#include <avr/io.h>
#include <util/delay.h>


//---------------- AVR PORT mapping ----------------//
#define		PORT_CLK_OUT	PORTD
#define		PORT_CLK_IN	PIND
#define		PORT_DAT_OUT	PORTD
#define		PORT_DAT_IN	PIND


//---------------- AVR bit names ----------------//
#define		BIT_CLK_OUT	0x80
#define		BIT_DAT_OUT	0x20
#define		BIT_CLK_IN	0x08
#define		BIT_DAT_IN	0x02


//---------------- code macros ----------------//
#define		COM_ACTIVE	( PORT_CLK_IN & BIT_CLK_IN )
#define		COM_DATA	( (PORT_DAT_IN & BIT_DAT_IN) / BIT_DAT_IN )


//---------------- functions and procedures ----------------//
extern uint16_t receive_com( int8_t bits );
extern void transmit_com( uint16_t data , int8_t bits );


//---------------- EOF ----------------//
#endif // COM
