/*
 * software / hardware interface of the midi switcher
 * @author haggl
*/

#include "gpio.h"



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

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
}

void configUSART( void )
{
    // USART
    UCSRB = 0x90; // enable RXEN and EXEN_vect
    UBRRH = UBRRHVAL;
    UBRRL = UBRRLVAL;
}

int8_t getButtonNumber( void )
{
    // input vector
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

void setOutputs(uint16_t data)
{
    PORTA = 0xf0 | (data & 0x00f)>>0;
    PORTB = 0xf0 | (data & 0x0f0)>>4;
    PORTC = 0xf0 | (data & 0xf00)>>8;
}
