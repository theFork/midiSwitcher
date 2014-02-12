/*
 * header for the program module
 * @author haggl
*/

#ifndef _PROGRAM
#define _PROGRAM


//---------------- includes ----------------//
#include <avr/eeprom.h>
#include <util/delay.h>


//---------------- data types ----------------//
// program data
typedef union {
    uint16_t word;
    struct {
        uint16_t    looper5     : 1;
        uint16_t    looper4     : 1;
        uint16_t    looper6     : 1;
        uint16_t    looper7     : 1;
        uint16_t    impulse1    : 1;
        uint16_t    impulse0    : 1;
        uint16_t    switch1     : 1;
        uint16_t    switch0     : 1;
        uint16_t    looper2     : 1;
        uint16_t    looper3     : 1;
        uint16_t    looper1     : 1;
        uint16_t    looper0     : 1;
    } channels;
} program_data_t;

// program
typedef struct
{
    uint8_t         number;
    program_data_t  data;
} program_t;


//---------------- functions and procedures ----------------//
void applyProgramData( program_data_t program );
void copyCurrentBankTo( uint8_t pgm );
void copyCurrentProgramTo( uint8_t pgm );
void enterProgram( uint8_t );
uint16_t readProgram( uint8_t );
void toggleChannel(uint8_t number);
void updateProgram( uint8_t number, uint16_t config );
void wipeCurrentProgram( void );
void wipeCurrentBank( void );


//---------------- EOF ----------------//
#endif // _PROGRAM
