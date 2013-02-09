/*
 * header for the midi switcher
 * @author haggl
 * @version 0.6.0 (7): 2011-09-02
*/

#ifndef _MIDI_SWITCHER
#define _MIDI_SWITCHER


//---------------- includes ----------------//
#include <avr/eeprom.h>
#include <avr/interrupt.h> 
#include <avr/io.h>
#include <util/delay.h>
#include <com.h>


//---------------- PORT mapping ----------------//
#define		COM_B3		7
#define		COM_B2		5
#define		COM_B1		3
#define		COM_B0		1


//---------------- COM commands ----------------//
#define		DISPLAY_MSG	0x02
#define		PROG_CHANGE	0x03
#define		COPY_PROGRAM	0x04
#define		COPY_BANK	0x05
#define		WIPE_PROGRAM	0x06
#define		WIPE_BANK	0x07


//---------------- data types ----------------//
typedef uint8_t bool;

volatile struct
{
	uint8_t program;
	uint16_t config : 12;
	bool progChange;
} state;


//---------------- functions and procedures ----------------//
// engine.c
extern void copyBank( uint8_t pgm );
extern void copyProgram( uint8_t pgm );
extern void init( void );
extern void sendPC( uint8_t );
extern void wipeProgram( void );
extern void wipeBank( void );

// interface.c
extern void configPorts( void );
extern void execProgram( void );
extern int8_t getButtonNumber( void );
extern void loadConfig( uint8_t );
extern void loadProgram( uint8_t );
extern void storeProgram( void );


//---------------- EOF ----------------//
#endif // _MIDI_SWITCHER
