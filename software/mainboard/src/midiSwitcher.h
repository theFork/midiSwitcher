/*
 * header for the midi switcher
 * @author haggl
*/

#ifndef _MIDI_SWITCHER
#define _MIDI_SWITCHER


//---------------- includes ----------------//
#include <avr/interrupt.h>
#include <com.h>


//---------------- PORT mapping ----------------//
#define        COM_B3           7
#define        COM_B2           5
#define        COM_B1           3
#define        COM_B0           1


//---------------- COM commands ----------------//
#define        DISPLAY_MSG      0x02
#define        PROG_CHANGE      0x03
#define        COPY_PROGRAM     0x04
#define        COPY_BANK        0x05
#define        WIPE_PROGRAM     0x06
#define        WIPE_BANK        0x07


//---------------- data types ----------------//
// execution state
typedef struct
{
    uint8_t progChange;
} exec_state_t;


//---------------- functions and procedures ----------------//
// interface.c
void configPorts( void );
void configUSART( void );
int8_t getButtonNumber( void );


//---------------- EOF ----------------//
#endif // _MIDI_SWITCHER
