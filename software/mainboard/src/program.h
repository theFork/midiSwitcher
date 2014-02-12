/*
 * header for the program module
 * @author haggl
*/

#ifndef _PROGRAM
#define _PROGRAM


//---------------- includes ----------------//
#include <avr/eeprom.h>
#include <stdint.h>


//---------------- data types ----------------//
// program data
typedef union {
    uint16_t word;
    struct {
        uint16_t    loopers     : 8;    // looper channels
        uint16_t    impulses    : 2;    // impulse channels
        uint16_t    switches    : 2;    // switch channels
    } channels;
} program_data_t;

// program
typedef struct
{
    uint8_t         number;
    program_data_t  data;
} program_t;


//---------------- functions and procedures ----------------//
void copyCurrentBankTo( uint8_t pgm );
void copyCurrentProgramTo( uint8_t pgm );
void enterProgram( uint8_t );
uint16_t readProgram( uint8_t );
void updateProgram( uint8_t number, uint16_t config );
void wipeCurrentProgram( void );
void wipeCurrentBank( void );


//---------------- EOF ----------------//
#endif // _PROGRAM
