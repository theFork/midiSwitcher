/*
 * header for the dog display
 * @author haggl
 * @version 0.3.0 (3): 2011-11-20
*/

#ifndef DOG
#define DOG


//---------------- includes ----------------//
#include <avr/io.h>
#include <util/delay.h>


//---------------- AVR PORT mapping ----------------//
#define		PORT_E		PORTB
#define		PORT_RS		PORTB
#define		PORT_RW		PORTB

#define		PORT_D0		PORTD
#define		PORT_D1		PORTD
#define		PORT_D2		PORTD
#define		PORT_D3		PORTD
#define		PORT_D4		PORTD
#define		PORT_D5		PORTD
#define		PORT_D6		PORTD
#define		PORT_D7		PORTD
//#define		PORT_E		PORTD
//#define		PORT_RS		PORTD
//#define		PORT_RW		PORTB
//
//#define		PORT_D0		PORTD
//#define		PORT_D1		PORTB
//#define		PORT_D2		PORTB
//#define		PORT_D3		PORTD
//#define		PORT_D4		PORTD
//#define		PORT_D5		PORTD
//#define		PORT_D6		PORTD
//#define		PORT_D7		PORTD


//---------------- AVR bit names ----------------//
#define		DOG_E		0
#define		DOG_RS		1
#define		DOG_RW		2

#define		DOG_D0		7
#define		DOG_D1		6
#define		DOG_D2		5
#define		DOG_D3		4
#define		DOG_D4		3
#define		DOG_D5		2
#define		DOG_D6		1
#define		DOG_D7		0
//#define		DOG_E		6
//#define		DOG_RS		7
//#define		DOG_RW		0
//
//#define		DOG_D0		5
//#define		DOG_D1		7
//#define		DOG_D2		6
//#define		DOG_D3		4
//#define		DOG_D4		3
//#define		DOG_D5		2
//#define		DOG_D6		1
//#define		DOG_D7		0


//---------------- functions and procedures ----------------//
extern void blinkCursor( void );
extern void clear( void );
extern void display( char* );
extern void home( void );
extern void home_lf( void );
extern void hideCursor( void );
extern void initDOG( void );
extern void moveTo( uint8_t );
extern void printChar( char );
extern void printHead( char* );
extern void printString( char* );
extern void printTail( char* );
extern void selectData( void );
extern void selectSys( void );
extern void shiftLeft( void );
extern void shiftRight( void );
extern void showCursor( void );
extern void sysCall( char );
extern void transmit( uint8_t );
extern void trigger( void );


//---------------- EOF ----------------//
#endif // DOG
