/*
 * EEPROM program storage service functions
 * @author haggl
*/

#include "program.h"
#include "gpio.h"



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

void applyProgramData(program_data_t data)
{
    // set outputs
    setOutputs(data.word);

    // clear impulse channels
    if (data.channels.impulse0 || data.channels.impulse1) {
        _delay_ms(10);

        data.channels.impulse0 = 0;
        data.channels.impulse1 = 0;

        setOutputs(data.word);
    }
}

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
    // if the program has not changed, do nothing
    if (current_program.number == num) {
        return;
    }

    // send impulses on the impulse channels
    if (current_program.data.channels.impulse0 || current_program.data.channels.impulse1) {
        applyProgramData(current_program.data);
        _delay_ms(10);
    }

    // load program and set outputs
    current_program.number = num;
    current_program.data.word = readProgram(num);
    applyProgramData(current_program.data);
}


uint16_t readProgram( uint8_t num )
{
    return eeprom_read_word(&programs[num]);
}

void toggleChannel(uint8_t number) {
    // compute new program data
    program_data_t data;
    data.word = current_program.data.word ^ (1<<number);
    program_data_t output = data;

    // determine if there is an impulse to send
    if (data.channels.looper0 != current_program.data.channels.looper0
    ||  data.channels.looper1 != current_program.data.channels.looper1
    ||  data.channels.looper2 != current_program.data.channels.looper2
    ||  data.channels.looper3 != current_program.data.channels.looper3
    ||  data.channels.looper4 != current_program.data.channels.looper4
    ||  data.channels.looper5 != current_program.data.channels.looper5
    ||  data.channels.looper6 != current_program.data.channels.looper6
    ||  data.channels.looper7 != current_program.data.channels.looper7
    ||  data.channels.switch0 != current_program.data.channels.switch0
    ||  data.channels.switch1 != current_program.data.channels.switch1) {
        output.channels.impulse0 = 0;
        output.channels.impulse1 = 0;
    }
    else {
        output.channels.impulse0 ^= current_program.data.channels.impulse0;
        output.channels.impulse1 ^= current_program.data.channels.impulse1;
    }

    // apply the new program data
    applyProgramData(output);

    // update the current program
    current_program.data.word = data.word;
    updateProgram(current_program.number, current_program.data.word);
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
