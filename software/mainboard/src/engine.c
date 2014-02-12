/*
 * major routines for the midi switcher
 * @author haggl
 * @version 1.0.1 (8): 2011-11-19
*/

#include "midiSwitcher.h"

extern exec_state_t state;


void copyProgram( uint8_t pgm )
{
    // save current program number
    uint8_t oldpgm = state.programNumber;

    // copy program
    state.programNumber = pgm;
    storeProgram();

    // reset state
    state.programNumber = oldpgm;
}

void copyBank( uint8_t targetBank )
{
    // compute the addresses of source and target programs
    uint8_t source = ( (state.programNumber+1) / 10 ) * 10;
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
                state.config.word = 0x000;
        }
        else readProgram( source + i );

        state.programNumber = targetBank + i;
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
    state.config.word = 0x000;
    execProgram();
    storeProgram();
}

void wipeBank( void )
{
    // clear config
    state.config.word = 0x000;
    execProgram();

    // write programs
    uint8_t bank = ( (state.programNumber+1) / 10 ) * 10;
    uint8_t i;
    for (i=0; i<10; i++)
    {
        state.programNumber = bank + i;
        storeProgram();
    }
}
