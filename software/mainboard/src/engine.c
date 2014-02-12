/*
 * major routines for the midi switcher
 * @author haggl
 * @version 1.0.1 (8): 2011-11-19
*/

#include "midiSwitcher.h"

extern exec_state_t state;


void init( void )
{
    // configure in-/out- ports
    configPorts();

    // configure in-/out- ports
    configUSART();

    // initialize variables
    state.progChange = 0;

    // fetch dump command to sync COM interface
    receive_com( 1 );
}

void sendPC( uint8_t pgm )
{
    transmit_com( PROG_CHANGE<<7 | pgm , 10 );
}
