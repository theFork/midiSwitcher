/* some useful vim commands
:let @x='yy@"j'
:map <C-b> :wa<CR>:!make backup<CR>
:map <C-d> :wa<CR>:!make dump<CR>
:map <C-m> :wa<CR>:!make clean build<CR>
:map <C-p> :wa<CR>:!make all<CR>
:let @c="0i//\ej"
:let @u='02xj'
*/

/*
 * main program of the midi switcher
 * @author haggl
*/

#include "midiSwitcher.h"
#include "program.h"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

extern program_t current_program;

// main state variable struct
exec_state_t    state;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

// initialization and endless loop
int main( void )
{
    // initialize and activate interrupts
    init();
    sei();

    // main program
    int8_t pressedSW;
    while ( 1 )
    {
        pressedSW = getButtonNumber();
        if ( pressedSW >= 0 && pressedSW <= 11 )
        {
            // update and execute the current program
            current_program.data.word ^= 1<<pressedSW;
            updateProgram(current_program.number, current_program.data.word);
            enterProgram(current_program.number);
        }

        if ( COM_ACTIVE )
        {
            // fetch package
            uint16_t data = receive_com( 10 );

            // extract command and program/bank number
            uint8_t cmd = data >> 7;
            uint8_t pgm = data & 0x7f;

            switch ( cmd )
            {
                case PROG_CHANGE:
                    enterProgram( pgm );
                    break;

                case COPY_PROGRAM:
                    copyCurrentProgramTo( pgm );
                    break;

                case COPY_BANK:
                    copyCurrentBankTo( pgm );
                    break;

                case WIPE_PROGRAM:
                    wipeCurrentProgram();
                    break;

                case WIPE_BANK:
                    wipeCurrentBank();
                    break;
            }
        }
    }

    return 0;
}

// INTERRUPT-FUNCTIONS
// midi package received
ISR(USART_RXC_vect)
{
    // wait until transmission is complete
    while ( !(UCSRA & 1<<RXC) );

    // fetch data
    uint8_t data = UDR;

    if ( state.progChange == 1 ) // PC flag set
    {
        state.progChange = 0;
        if ( data < 120 )
        {
            enterProgram( data );
            sendPC( data );
        }
    }
    else if ( (data & 0xF0) == 0xC0 ) // in case of PC, set flag
        state.progChange = 1;
}
