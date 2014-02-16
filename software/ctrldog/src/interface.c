/*
 * software / hardware interface of the control module
 * @author haggl
 * @version 0.3.1 (6): 2011-11-19
*/

#include "ctrldog.h"


// EEPROM variables
int8_t initpnum_eemem EEMEM;
uint8_t pnum_eemem EEMEM;
unsigned char nameTail_eemem[12][12] EEMEM;
unsigned char nameHead_eemem[12][12] EEMEM;

void configPorts( void )
{
	DDRB = 0x47;	// [7] button input, [6] debug output, [2:0] DOG outputs (E, RS, RW)
	PORTB |= 0x80;	// activate pull-up on [7]
	PORTB &=~ 0x07;	// drive DOG outputs low
	PORTB |= 0x40;	// drive debug output high

	DDRC = 0x03;	// [5:4] encoder inputs, [3:2] COM in, [1:0] COM out
	PORTC |= 0x30;	// activate pull-ups on encoder inputs
	PORTC &=~ 0x03;	// drive outputs low
//	DDRB = 0xc1;	// setup [7:6] and [0] as outputs
//	PORTB = 0x06;	// activate pull-ups on [2:1]
//	PORTB &=~ 0xc1;	// drive outputs low
//
//	DDRC = 0x23;	// [4] encoder_OK input, [3:2] COM in, [1:0] COM out
//	PORTC = 0x10;	// activate pull-up on encoder_OK input
//	PORTC &=~ 0x03;	// drive outputs low

	DDRD = 0xff;	// DOG data outputs
	PORTD = 0x00;	// drive outputs low
}

bool ctrlOK_trig( void )
{
	if ( !(PIN_OK & (1 << CTRL_OK)) )
	{
		_delay_ms( 100 );
		while ( !(PIN_OK & (1 << CTRL_OK)) );
		return 1;
	}
	return 0;
}

uint8_t ctrlImp_trig( void )
{
	if ( state.siga == A_HIGH && state.sigb == B_HIGH ) // position unchanged
		return NO_ACTION;

	// determine direction and update counter
	uint8_t action = NO_ACTION;
	if ( (!state.siga && !state.sigb &&  A_HIGH) || // direction: down
	     ( state.siga && !state.sigb &&  B_HIGH) ||
	     ( state.siga &&  state.sigb && !A_HIGH) ||
	     (!state.siga &&  state.sigb && !B_HIGH) )
	{
		action = CTRL_DN_ACTION;
		state.counter--;
	}
	else if ( (!state.siga && !state.sigb &&  B_HIGH) || // direction: up
	     ( state.siga && !state.sigb && !A_HIGH) ||
	     ( state.siga &&  state.sigb && !B_HIGH) ||
	     (!state.siga &&  state.sigb &&  A_HIGH) )
	{
		action = CTRL_UP_ACTION;
		state.counter++;
	}

	if ( state.counter == -3 || state.counter == 3 ) // third impulse received
	{
		// de-bounce and reinitialize state variables
		_delay_ms( 25 );
		state.siga = A_HIGH;
		state.sigb = B_HIGH;
		state.counter = 0;

		return action;
	}


	// just update state variables
	state.siga = A_HIGH;
	state.sigb = B_HIGH;

	return NO_ACTION;
}

void debugSignal( void )
{
	PORTB &=~ 0x40;
	_delay_ms(500);
	PORTB |= 0x40;
	_delay_ms(50);
}

void loadProgram( uint8_t pnum )
{
	// set program number
	program.number = pnum; 

	// save program number
	eeprom_write_byte( &pnum_eemem, pnum );

	// read program name
	uint8_t i;
	for (i=0; i<12; i++)
	{
		program.nameHead[i] = eeprom_read_byte( &nameHead_eemem[(pnum+1)/10][i] );
		program.nameTail[i] = eeprom_read_byte( &nameTail_eemem[(pnum+1)/10][i] );
	}
}

uint8_t loadProgramNumber( void )
{
	return eeprom_read_byte( &pnum_eemem );
}

int8_t loadInitProgramNumber( void )
{
	return eeprom_read_byte( (uint8_t*) &initpnum_eemem );
}

void storeBank( uint8_t bankNumber )
{
	// write bank name
	uint8_t i;
	for (i=0; i<12; i++)
	{
		eeprom_write_byte( &nameHead_eemem[bankNumber][i], program.nameHead[i] );
		eeprom_write_byte( &nameTail_eemem[bankNumber][i], program.nameTail[i] );
	}
}

void storeInitProgramNumber( int8_t pnum )
{
	eeprom_write_byte( (uint8_t*) &initpnum_eemem, pnum );
}

void wipeBank( void )
{
	// signal midi switcher
	transmit_com( WIPE_BANK<<7 , 10 );

	// clear bank name
	uint8_t bank = (program.number+1) / 10; 

	uint8_t i;
	for (i=0; i<12; i++)
	{
		eeprom_write_byte( &nameHead_eemem[bank][i], ' ' );
		eeprom_write_byte( &nameTail_eemem[bank][i], ' ' );
		program.nameHead[i] = ' ';
		program.nameTail[i] = ' ';
	}
}
