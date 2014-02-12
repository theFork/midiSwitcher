/*
 * software / hardware interface of the midi switcher
 * @author haggl
 * @version 1.0.1 (9): 2012-03-25
*/

#include "midiSwitcher.h"

extern exec_state_t state;

uint16_t programs[120] EEMEM;

void configPorts( void )
{
    // switch and relay/LED ports
    DDRA = 0x0f;    // [7:4] in; [3:0] out
    PORTA = 0xf0;    // activate pull-up resistors for inputs
    DDRB = 0x0f;    // [7:4] in; [3:0] out
    PORTB = 0xf0;    // activate pull-up resistors for inputs
    DDRC = 0x0f;    // [7:4] in; [3:0] out
    PORTC = 0xf0;    // activate pull-up resistors for inputs
    DDRD = 0xa0;    // [7,5] COM out, [3,1] COM in ; [0] rxd
    PORTD = 0x00;    // deactivate all pull-up resistors

    // USART
    UCSRB = 0x90; // enable RXEN and EXEN_vect
    #define BAUDRATE 31250
    #define UBRRHVAL (uint8_t)((F_OSC/BAUDRATE/16-1)>>8)
    #define UBRRLVAL (uint8_t)((F_OSC/BAUDRATE/16-1))
    UBRRH = UBRRHVAL;
    UBRRL = UBRRLVAL;
}

void execProgram( void )
{
    PORTA = 0xf0 | (state.config & 0x00f)>>0;
    PORTB = 0xf0 | (state.config & 0x0f0)>>4;
    PORTC = 0xf0 | (state.config & 0xf00)>>8;
}

int8_t getButtonNumber( void )
{
    // input vector
    #define ACTIVEPINS ((~PINC & 0xf0)<<4) | (~PINB & 0xf0) | ((~PINA & 0xf0)>>4)
    int vector = ACTIVEPINS;

    if (vector)
    {
        // compute the button number
        int result = 0;
        while (vector % 2 == 0)
        {
            vector >>= 1;
            result++;
        }

        // multiple switches pressed -> error
        vector--;
        if(vector != 0) return -1;

        // de-bounce
        _delay_ms(100);
        while (ACTIVEPINS);

        return result;
    }
    return -1;
}

void loadConfig( uint8_t num )
{
    state.config = eeprom_read_word(&programs[num]);
}

void loadProgram( uint8_t num )
{
    state.programNumber = num;
    loadConfig( num );
    execProgram();
}

void storeProgram( void )
{
    eeprom_write_word(&programs[state.programNumber], state.config);
}
