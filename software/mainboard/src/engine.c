/*
 * major routines for the midi switcher
 * @author haggl
 * @version 1.0.1 (8): 2011-11-19
*/

#include "midiSwitcher.h"


void copyProgram( uint8_t pgm )
{
    // save current program number
    uint8_t oldpgm = state.program;

    // copy program
    state.program = pgm;
    storeProgram();

    // reset state
    state.program = oldpgm;
}

void copyBank( uint8_t targetBank )
{
    // compute the addresses of source and target programs
    uint8_t source = ( (state.program+1) / 10 ) * 10;
    targetBank *= 10;

    // write programs
    uint8_t i;
    for (i=0; i<10; i++)
    {
        // bank 0 contains only 9 programs, so we have to skip certain actions
        if ( i == 9 )
        {
            if ( targetBank == 0 ) // bank 0 is the target
                break;

            if ( source == 0 ) // bank 0 is the source
                state.config = 0x000;
        }
        else loadConfig( source + i );

        state.program = targetBank + i;
        storeProgram();
    }
}

void init( void )
{
    // configure in-/out- ports
    configPorts();

    // initialize variables
    state.progChange = 0;

    // fetch dump command to sync COM interface
    receive_com( 1 );
}

void sendPC( uint8_t pgm )
{
    transmit_com( PROG_CHANGE<<7 | pgm , 10 );
}

void wipeProgram( void )
{
    state.config = 0x000;
    execProgram();
    storeProgram();
}

void wipeBank( void )
{
    // clear config
    state.config = 0x000;
    execProgram();

    // write programs
    uint8_t bank = ( (state.program+1) / 10 ) * 10;
    uint8_t i;
    for (i=0; i<10; i++)
    {
        state.program = bank + i;
        storeProgram();
    }
}
