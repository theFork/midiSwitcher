/*
 * EEPROM program storage service functions
 * @author haggl
*/

#include "program.h"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

// current program
program_t current_program;

// EEPROM program storage
uint16_t programs[120] EEMEM;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void copyCurrentProgramTo( uint8_t pgm )
{
    // copy program
    updateProgram(pgm, current_program.data.word);
}

void copyCurrentBankTo( uint8_t targetBank )
{
    // compute the source bank
    uint8_t sourceBank = ( (current_program.number+1) / 10 ) * 10;

    // write programs
    uint16_t data;
    uint8_t program_index;
    for (program_index=0; program_index<10; program_index++)
    {
        // bank 0 contains only 9 programs, so we have to skip certain actions
        if ( program_index == 9 )
        {
            if ( targetBank == 0 ) // bank 0 is the target
                break;

            if ( sourceBank == 0 ) // bank 0 is the source
                data = 0;
        }
        else data = readProgram(sourceBank + program_index);

        updateProgram((targetBank*10) + program_index, data);
    }
}

void enterProgram( uint8_t num )
{
    // load program
    current_program.number = num;
    current_program.data.word = readProgram( num ) ;

    // set outputs
    PORTA = 0xf0 | (current_program.data.word & 0x00f)>>0;
    PORTB = 0xf0 | (current_program.data.word & 0x0f0)>>4;
    PORTC = 0xf0 | (current_program.data.word & 0xf00)>>8;
}

uint16_t readProgram( uint8_t num )
{
    return eeprom_read_word(&programs[num]);
}

void updateProgram(uint8_t number, uint16_t data)
{
    eeprom_write_word(&programs[number], data);
}

void wipeCurrentProgram(void)
{
    updateProgram(current_program.number, 0);
    enterProgram(current_program.number);
}

void wipeCurrentBank(void)
{
    // write programs
    uint8_t bank = ( (current_program.number+1) / 10 ) * 10;
    uint8_t i;
    for (i=0; i<10; i++) {
        updateProgram(bank+i, 0);
    }
    
    // apply the changes
    enterProgram(current_program.number);
}
