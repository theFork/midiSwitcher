/*
 * 2-wire communication protocol for the midi switcher
 * @author haggl
*/

#include "com.h"

uint16_t receive_com( int8_t bits )
{
    uint16_t data = 0;
    for (bits--; bits >= 0; bits--)
    {
        // read data bit
        data |= COM_DATA<<bits;
        while ( COM_ACTIVE );

        if ( bits == 0 ) // last bit received
        {
            return data;
        }

        // wait for next bit
        while ( !COM_ACTIVE );
    }
    return data; // never reached
}

void transmit_com( uint16_t data , int8_t bits )
{
    for (bits--; bits >= 0; bits--)
    {
        // set / clear data bit
        if ( data>>bits & 0x1 )
            PORT_DAT_OUT |= BIT_DAT_OUT;
        else
            PORT_DAT_OUT &=~ BIT_DAT_OUT;

        // transmit
        PORT_CLK_OUT |= BIT_CLK_OUT;
        _delay_us(500);
        PORT_CLK_OUT &=~ BIT_CLK_OUT;
        _delay_us(500);
    }
}
