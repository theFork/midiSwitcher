/*
 * header for the gpio interface
 * @author haggl
*/

#ifndef _GPIO
#define _GPIO


//---------------- includes ----------------//
#include <avr/io.h>
#include <util/delay.h>


//---------------- constants and macros ----------------//
// USART
#define BAUDRATE 31250
#define UBRRHVAL (uint8_t)((F_OSC/BAUDRATE/16-1)>>8)
#define UBRRLVAL (uint8_t)((F_OSC/BAUDRATE/16-1))

// buttons
#define ACTIVEPINS ((~PINC & 0xf0)<<4) | (~PINB & 0xf0) | ((~PINA & 0xf0)>>4)


//---------------- functions and procedures ----------------//
// interface.c
void configPorts( void );
void configUSART( void );
int8_t getButtonNumber( void );
void setOutputs( uint16_t data );


//---------------- EOF ----------------//
#endif // _GPIO
