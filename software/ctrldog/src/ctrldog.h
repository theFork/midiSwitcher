/*
 * main header for the control module
 * @author haggl
 * @version 0.3.1 (6): 2011-11-19
*/

#ifndef _CTRLDOG
#define _CTRLDOG


//---------------- includes ----------------//
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>
#include "dog.h"
#include "com.h"


//---------------- data types and global variables ----------------//
typedef uint8_t bool;

// codes for action()
enum action_t { NO_ACTION, CTRL_DN_ACTION, CTRL_OK_ACTION, CTRL_UP_ACTION, COM_ACTION };

// prompt types
enum prompt_t { PROGRAM_PROMPT, BANK_PROMPT };

// current program number and name
volatile struct
{
	uint8_t number;
	char nameHead[12];
	char nameTail[12];
} program;

// program state variables
volatile struct
{
	// encoder state
	bool siga;
	bool sigb;
	int8_t counter;

	uint8_t prev_pnum;
} state;


//---------------- AVR PORT mapping ----------------//
#define		PIN_SIGA	PINC
#define		PIN_SIGB	PINC
#define		PIN_OK		PINB
//#define		PIN_SIGA	PINB
//#define		PIN_SIGB	PINB
//#define		PIN_OK		PINC


//---------------- AVR bit names ----------------//
#define		CTRL_SIGA	4
#define		CTRL_SIGB	5
#define		CTRL_OK		7
//#define		CTRL_SIGA	1
//#define		CTRL_SIGB	2
//#define		CTRL_OK		4


//---------------- COM commands ----------------//
#define		DEBUG_MSG	0x02
#define		PROG_CHANGE	0x03
#define		COPY_PROGRAM	0x04
#define		COPY_BANK	0x05
#define		WIPE_PROGRAM	0x06
#define		WIPE_BANK	0x07


//---------------- code macros ----------------//
#define		A_HIGH		!( PIN_SIGA>>CTRL_SIGA & 1) 
#define		B_HIGH		!( PIN_SIGB>>CTRL_SIGB & 1)
#define		COUNT_UP	if (program.number < 118) program.number++; else program.number = 0;
#define		COUNT_DN	if (program.number > 0)   program.number--; else program.number = 118;
#define		DEBUG1		display( "     debug1     " ); _delay_ms( 500 ); display( "                " ); _delay_ms( 500 )
#define		DEBUG2		display( "     debug2     " ); _delay_ms( 500 ); display( "                " ); _delay_ms( 500 )


//---------------- constants ----------------//
#define		MENU_DEFAULT	2


//---------------- functions and procedures ----------------//
// engine.c
extern enum action_t action( void );
extern void copyProgram( void );
extern void enterProgram( uint8_t );
extern void execCMD( uint16_t );
extern void init( void );
extern void menu( void );
extern bool prompt( char*, uint8_t, uint8_t );
extern void rename( void );
extern void selectProgram( void );
extern void sendPC( uint8_t );
extern int8_t setInitProgram( void );

// interface.c
extern void configPorts( void );
extern bool ctrlOK_trig( void );
extern uint8_t ctrlImp_trig( void );
extern void debugSignal( void );
extern void loadProgram( uint8_t );
extern uint8_t loadProgramNumber( void );
extern int8_t loadInitProgramNumber( void );
extern void storeBank( uint8_t );
extern void storeInitProgramNumber( int8_t );
extern void wipeBank( void );


//---------------- EOF ----------------//
#endif // _CTRLDOG
