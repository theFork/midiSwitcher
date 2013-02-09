/*
 * library for the EA DOGM162 LED display
 * @author haggl
 * @version 0.2.0 (2): 2011-11-20
*/

#include "dog.h"


void blinkCursor( void )
{
	sysCall( 0x0d );
}

void clear( void )
{
	sysCall( 0x01 );
}

void display( char* str )
{
	clear();
	home();
	while (*str != '\0')
	{
		if (*str != '\a')
			printChar( *str );
		else
			home_lf();

		str++;
	}
}

void hideCursor()
{
	sysCall( 0x0c );
}

void home( void )
{
	sysCall( 0x02 );
}

void home_lf( void )
{
	sysCall( 0xc0 );
}

void initDOG( void )
{
	// init sequence (-> data sheet)
	_delay_ms( 50 );
	PORT_RW &=~ (1<<DOG_RW); // write mode
	sysCall( 0x39 ); // 8-bit mode, 2 lines, IS[01]
	sysCall( 0x1c ); // bias 1/4
	sysCall( 0x69 ); // follower control
	sysCall( 0x52 ); // booster off, contrast [6:5] = [10]
	sysCall( 0x74 ); // contrast [4:0] = [0100]
	sysCall( 0x06 ); // entry mode
	sysCall( 0x38 ); // switch to IS[00]
	sysCall( 0x01 ); // clear display, cursor -> home
	sysCall( 0x0c ); // turn display on
}

void moveTo( uint8_t pos )
{
	sysCall( pos | 0x80 );
}

void printChar( char data )
{
	selectData();
	transmit( data );
}

void printHead( char* str )
{
	clear();
	home();
	printString( str );
}

void printString( char* str )
{
	while (*str != '\0')
	{
		printChar( *str );
		str++;
	}
}

void printTail( char* str )
{
	home_lf();
	printString( str );
}

void selectData( void )
{
	PORT_RS |= 1<<DOG_RS;
}

void selectSys( void )
{
	PORT_RS &=~ 1<<DOG_RS;
} 

void shiftLeft( void )
{
	sysCall( 0x18 );
}

void shiftRight( void )
{
	sysCall( 0x1c );
}

void showCursor()
{
	sysCall( 0x0e );
}

void sysCall( char cmd )
{
	selectSys();
	transmit( cmd );
}

void transmit( uint8_t data )
{
	// clear PORTs
	PORT_D7 &=~ (1<<DOG_D7);
	PORT_D6 &=~ (1<<DOG_D6);
	PORT_D5 &=~ (1<<DOG_D5);
	PORT_D4 &=~ (1<<DOG_D4);
	PORT_D3 &=~ (1<<DOG_D3);
	PORT_D2 &=~ (1<<DOG_D2);
	PORT_D1 &=~ (1<<DOG_D1);
	PORT_D0 &=~ (1<<DOG_D0);

	// set data bits
	PORT_D7 |= ((data & 0x80) >> 7) << DOG_D7;
	PORT_D6 |= ((data & 0x40) >> 6) << DOG_D6;
	PORT_D5 |= ((data & 0x20) >> 5) << DOG_D5;
	PORT_D4 |= ((data & 0x10) >> 4) << DOG_D4;
	PORT_D3 |= ((data & 0x08) >> 3) << DOG_D3;
	PORT_D2 |= ((data & 0x04) >> 2) << DOG_D2;
	PORT_D1 |= ((data & 0x02) >> 1) << DOG_D1;
	PORT_D0 |= ((data & 0x01) >> 0) << DOG_D0;

	// trigger read/write
	trigger();
}

void trigger( void )
{
	PORT_E |= 1<<DOG_E;
	_delay_us( 750 );
	PORT_E &=~ (1<<DOG_E);
}
