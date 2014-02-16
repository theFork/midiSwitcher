/*
 * major routines for the control module
 * @author haggl
 * @version 0.3.0 (7): 2011-11-20
 */

#include "ctrldog.h"


char digits[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

char welcome[] =
{
	0x0a, 0x5f, 0x0c, 0x0a, 0x5f, 0x5f, 0x0c, 0xff,
	0x0a, 0x0c, 0xff, 0xff, 0x0a, 0x5f, 0x0c, 0x0a,
	'\a',
	'~', 'w', 'w', 'w', '.', 's', 'i', 'n',
	'e', 'b', 'a', 'n', 'd', '.', 'd', 'e',
	'\0'
};

enum action_t action( void )
{
	if ( COM_ACTIVE ) // incoming program change
		return COM_ACTION;

	if ( ctrlOK_trig() )
		return CTRL_OK_ACTION;

	return ctrlImp_trig();
}

void copyBank( void )
{
	printHead( "   copy bank" );

	// print source- and destination bank numbers
	uint8_t bank = (program.number+1)/10;
	char label[15] = "  #000 => #000";
	label[4] = digits[(bank % 100) / 10];
	label[5] = digits[bank % 10];
	label[12] = digits[(bank % 100) / 10];
	label[13] = digits[bank % 10];
	printTail( label );

	while ( 1 ) // wait for input
	{
		switch ( action() )
		{
			case CTRL_DN_ACTION:
				if (bank > 0)   bank--; else bank = 11;
				break;
			case CTRL_UP_ACTION:
				if (bank < 11) bank++; else bank = 0;
				break;
			case CTRL_OK_ACTION:
				transmit_com( COPY_BANK<<7 | bank , 10 ); // signal switcher
				storeBank( bank );
				return;
			default:
				continue;
		}

		// update display
		label[12] = digits[(bank % 100) / 10];
		label[13] = digits[bank % 10];
		printTail( label );
	}
}

void copyProgram( void )
{
	printHead( "  copy program" );

	// print source and destination program numbers
	uint8_t pnum = program.number+1;
	char label[] = "  00.0 => 00.0";
	label[2] = digits[pnum / 100];
	label[3] = digits[(pnum % 100) / 10];
	label[5] = digits[pnum % 10];
	label[10] = digits[pnum / 100];
	label[11] = digits[(pnum % 100) / 10];
	label[13] = digits[pnum % 10];
	printTail( label );

	while ( 1 )
	{
		switch ( action() )
		{
			case CTRL_DN_ACTION:
				COUNT_DN
				break;
			case CTRL_UP_ACTION:
				COUNT_UP
				break;
			case CTRL_OK_ACTION:
				// signal switcher
				transmit_com( COPY_PROGRAM<<7 | program.number , 10 );
				return;
			default:
				continue;
		}

		// update program number
		pnum = program.number+1;
		label[10] = digits[(pnum) / 100];
		label[11] = digits[((pnum) % 100) / 10];
		label[13] = digits[(pnum) % 10];
		printTail( label );
	}
}

void enterProgram( uint8_t pnum, bool send_pc )
{
	// signal midiSwitcher
	if (send_pc) {
		sendPC( pnum );
	}

	// save previous program
	state.prev_pnum = loadProgramNumber();

	// load new program
	loadProgram( pnum );

	// display current bank number and first slice of bank name
	uint8_t i;
	char label[] = "                ";
	pnum = program.number+1;
	label[0] = 'B';
	label[1] = digits[pnum / 100];
	label[2] = digits[(pnum % 100) / 10];
	for (i=0; i<12; i++)
		label[i+4] = program.nameHead[i];
	printHead( label );

	// display current program number and rest of bank name
	label[0] = 'P';
	label[1] = ' ';
	label[2] = digits[pnum % 10];
	for (i=0; i<12; i++)
		label[i+4] = program.nameTail[i];
	printTail( label );
}

void execCMD( uint16_t data )
{
	// extract command and program/bank number
	uint8_t cmd = data>>7;
	uint8_t num = data & 0x7f;

	// execute command
	if ( cmd == PROG_CHANGE && 0 <= num && num < 119 )
		enterProgram( num, false );

	else if ( cmd == DEBUG_MSG )
	{
		char label[] = " in: bbbbbbbbbb";
		label[5] = digits[data>>9];
		label[6] = digits[data>>8 & 0x1];
		label[7] = digits[data>>7 & 0x1];
		label[8] = digits[data>>6 & 0x1];
		label[9] = digits[data>>5 & 0x1];
		label[10] = digits[data>>4 & 0x1];
		label[11] = digits[data>>3 & 0x1];
		label[12] = digits[data>>2 & 0x1];
		label[13] = digits[data>>1 & 0x1];
		label[14] = digits[data & 0x1];
		printHead( label );
	}
}


void init( void )
{
	// currentProgram.configure in-/out- ports
	configPorts();
	initDOG();

	// initialize state variables
	state.prev_pnum = 0;
	state.siga = A_HIGH;
	state.sigb = B_HIGH;
	state.counter = 0;

	// fancy startup screen
	display( welcome );
	_delay_ms( 4000 );

	// enter previous or default program
	if (loadInitProgramNumber() > -1) enterProgram( loadInitProgramNumber(), true );
	else enterProgram( loadProgramNumber(), true );
}

void menu( void )
{
	// menu labels
	char menu_label[9][34] =
	{
		"\x20  main menu   ~\a   leave menu",
		"\x7f  main menu   ~\a switch program",
		"\x7f  main menu   ~\a select program",
		"\x7f  main menu   ~\a  copy program",
		"\x7f  main menu   ~\a  wipe program",
		"\x7f  main menu   ~\a  rename bank",
		"\x7f  main menu   ~\a   copy bank",
		"\x7f  main menu   ~\a   wipe bank",
		"\x7f  main menu    \aset init program"
	};

	uint8_t item = MENU_DEFAULT;
	bool done = 0;
	display( menu_label[item] );

	while ( 1 ) // wait for input
	{
		switch ( action() )
		{
			case CTRL_DN_ACTION:
				if ( item > 0 ) item--;
				display( menu_label[item] );
				break;
			case CTRL_UP_ACTION:
				if ( item < 8 ) item++;
				display( menu_label[item] );
				break;
			case CTRL_OK_ACTION:
				switch ( item )
				{
					case 0: // leave menu
						enterProgram( program.number, true );
						return;
					case 1: // switch program
						if ( prompt( "   switch to" , PROGRAM_PROMPT , state.prev_pnum+1 ) )
						{
							enterProgram( state.prev_pnum, true );
							return;
						}
						break;
					case 2: // select program
						selectProgram();
						return;
					case 3: // copy program
						copyProgram();
						done = 1;
						break;
					case 4: // wipe program
						if ( prompt( "  wipe program" , PROGRAM_PROMPT , program.number+1 ) )
						{
							transmit_com( WIPE_PROGRAM<<7 , 10 );
							done = 1;
						}
						break;
					case 5: // rename bank
						rename();
						storeBank( (program.number+1) / 10 );
						done = 1;
						break;
					case 6: // copy bank
						copyBank();
						done = 1;
						break;
					case 7: // wipe bank
						if ( prompt( "   wipe bank" , BANK_PROMPT , (program.number+1)/10 ) )
						{
							wipeBank();
							done = 1;
						}
						break;
					case 8: // set init program
						storeInitProgramNumber( setInitProgram() );
						done = 1;
						break;
				}

				// give feedback
				clear();
				if ( done )
				{
					printTail( "     done." );
					done = 0;
				}
				else printTail( "    aborted." );

				_delay_ms( 750 );
				item = MENU_DEFAULT;
				display( menu_label[item] );
				break;

			default:
				continue;
		}
	}
}

bool prompt( char* question, uint8_t type, uint8_t pnum )
{
	printHead( question );

	// display target program number
	char label[] = "     # .  ?";
	uint8_t i = 5 + type; // increment index if printing a "bank"-prompt
	label[i++] = digits[pnum / 100];
	label[i] = digits[(pnum % 100) / 10];
	label[8] = digits[pnum % 10];
	printTail( label );

	while ( 1 ) // wait for input
	{
		switch ( action() )
		{
			case CTRL_OK_ACTION:
				return 1;
			case CTRL_DN_ACTION:
				return 0;
			case CTRL_UP_ACTION:
				return 0;
			default:
				continue;
		}
	}
}

void rename( void )
{
	#define	HEAD_START	0x84
	#define	HEAD_END	0x8f
	#define	TAIL_START	0x44
	#define	TAIL_END	0x4f

	// print current bank name
	char label[] = "mv |            ";
	uint8_t i;
	for (i=0; i<12; i++)
		label[i+4] = program.nameHead[i];
	printHead( label );

	label[0] = ' ';
	label[1] = ' ';
	for (i=0; i<12; i++)
		label[i+4] = program.nameTail[i];
	printTail( label );

	// move to first character and show cursor
	uint8_t position = HEAD_START;
	moveTo( position );
	showCursor();

	bool edit = 0;
	bool exit = 0;
	while( 1 ) // wait for input
	{
		switch ( action() )
		{
			case CTRL_OK_ACTION:
				if ( !edit && !exit ) // normal move mode
				{
					// enter edit mode
					blinkCursor();
					edit = 1;

					// display edit mode
					moveTo( 0x80 );
					printChar( 'e' );
					printChar( 'd' );
					moveTo( position );
				}

				else if ( edit && !exit ) // normal edit mode
				{
					// set and indicate exit flag
					exit = 1;
					moveTo( 0x40 );
					printChar( 0x7f );

					// advance one character
					position++;
					moveTo( position );
				}

				else if ( edit && exit ) // edit-exit mode
				{
					// enter move-exit mode
					edit = 0;
					showCursor();

					// display move mode with exit indicator
					moveTo( 0x80 );
					printChar( 'm' );
					printChar( 'v' );
					moveTo( 0x40 );
					printChar( 0x7f );
					moveTo( position );
				}

				else if ( !edit && exit ) // move-exit mode
				{
					hideCursor();
					return;
				}

				break;

			case CTRL_DN_ACTION:
				if ( edit )
				{
					// get current character
					char* character;
					if ( position >= HEAD_START )
						character = &program.nameHead[position-HEAD_START];
					else
						character = &program.nameTail[position-TAIL_START];

					// use only the essential ascii characters
					if ( *character > 0x20 )
						--*character;
					else
						*character = 0x7f;

					printChar( *character );
				}
				else
				{
					// decrement position pointer (cyclic)
					if ( position == HEAD_START )
						position = TAIL_END;
					else if ( position == TAIL_START )
						position = HEAD_END;
					else position--;

				}

				if ( exit )
				{
					// enter normal mode
					moveTo( 0x40 );
					printChar( ' ' );
					exit = 0;
				}

				// update cursor display
				moveTo( position );
				break;

			case CTRL_UP_ACTION:
				if ( edit )
				{
					// get current character
					char* character;
					if ( position >= HEAD_START )
						character = &program.nameHead[position-HEAD_START];
					else
						character = &program.nameTail[position-TAIL_START];

					// use only the essential ascii characters
					if ( *character < 0x7f )
						++*character;
					else
						*character = 0x20;

					printChar( *character );
				}
				else
				{
					// increment position pointer (cyclic)
					if ( position == HEAD_END )
						position = TAIL_START;
					else if ( position == TAIL_END )
						position = HEAD_START;
					else position++;
				}

				if ( exit )
				{
					// enter normal mode
					moveTo( 0x40 );
					printChar( ' ' );
					exit = 0;
				}

				// update cursor display
				moveTo( position );
				break;

			default:
				continue;
		}
	}
}

void selectProgram( void )
{
	printHead( " select program" );

	// display target program number
	uint8_t pnum = program.number+1;
	char label[] = "     ~ 00.0";
	label[7] = digits[pnum / 100];
	label[8] = digits[(pnum % 100) / 10];
	label[10] = digits[pnum % 10];
	printTail( label );

	while ( 1 )
	{
		switch ( action() )
		{
			case CTRL_DN_ACTION:
				COUNT_DN
				break;
			case CTRL_UP_ACTION:
				COUNT_UP
				break;
			case CTRL_OK_ACTION:
				enterProgram( program.number, true );
				return;
			default:
				continue;
		}

		// update program number
		pnum = program.number+1;
		label[7] = digits[pnum / 100];
		label[8] = digits[(pnum % 100) / 10];
		label[10] = digits[pnum % 10];
		printTail( label );
	}
}

void sendPC( uint8_t pgm )
{
	transmit_com( PROG_CHANGE<<7 | pgm , 10 );
}

int8_t setInitProgram( void )
{
	printHead( "set init program" );

	// display current default program number
	int8_t pnum = loadInitProgramNumber()+1;
	char label[] = "     ~ 00.0";
	if (pnum != 0)
	{
		label[7] = digits[pnum / 100];
		label[8] = digits[(pnum % 100) / 10];
		label[9] = '.';
		label[10] = digits[pnum % 10];
	}
	else
	{
		label[7] = 'a';
		label[8] = 'u';
		label[9] = 't';
		label[10] = 'o';
	}
	printTail( label );

	while ( 1 )
	{
		switch ( action() )
		{
			case CTRL_DN_ACTION:
				if (pnum > 0) pnum--; else pnum = 119;
				break;
			case CTRL_UP_ACTION:
				if (pnum < 119) pnum++; else pnum = 0;
				break;
			case CTRL_OK_ACTION:
				return pnum-1;
			default:
				continue;
		}

		// update display
		if (pnum != 0)
		{
			label[7] = digits[pnum / 100];
			label[8] = digits[(pnum % 100) / 10];
			label[9] = '.';
			label[10] = digits[pnum % 10];
		}
		else
		{
			label[7] = 'a';
			label[8] = 'u';
			label[9] = 't';
			label[10] = 'o';
		}
		printTail( label );
	}
}
