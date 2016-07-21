/********************************************
*
*  Name: Arjun Narayan
*  Section: Tuesday/Thursday 2PM-3:20PM
*  Assignment: Final Project
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
void writenibble(unsigned char);

/*
  init_lcd - Do various things to initialize the LCD display
*/
void init_lcd()
{
    DDRD |= (1 << PD7);//configure port as output
    DDRD |= (1 << PD6);
    DDRD |= (1 << PD5);
    DDRD |= (1 << PD4);
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    _delay_ms(15);              // Delay at least 15ms
    writenibble(0x30); // Use writenibble to 78send 0011
    _delay_ms(5);               // Delay at least 4msec
    writenibble(0x30); // Use writenibble to send 0011
    _delay_us(120);             // Delay at least 100usec
    writenibble(0x30); // Use writenibble to send 0011, no delay needed
    writenibble(0x20); // Use writenibble to send 0010  // Function Set: 4-bit interface
    _delay_ms(2);
    writecommand(0x28);         // Function Set: 4-bit interface, 2 lines
    _delay_ms(2);
    writecommand(0xf0);         // Display and cursor on
    _delay_ms(2);
}

/*
  stringout - Print the contents of the character string "str"
  at the current cursor position.
*/
void stringout(char *str)
{
    int i;
    for(i = 0; *(str+i)!='\0'; i++){
        writedata(*(str+i));
    }
}

/*
  moveto - Move the cursor to the postion "pos"
*/
void moveto(unsigned char pos)
{
    writecommand(0x80 + pos);//add offset to first position on screen
}

/*
  writecommand - Output a byte to the LCD display instruction register.
*/
void writecommand(unsigned char x)
{
    PORTB &= ~(1 << PB0);//set register select to 0 (command register)
    writenibble(x);//send 4 msb in first (lsb cleared to 0 in writenibble)
    writenibble(x << 4);//shift left logical by 4 to send in lsb
    _delay_ms(2);
}

/*
  writedata - Output a byte to the LCD display data register
*/
void writedata(unsigned char x)
{
    PORTB |= (1 << PB0);//set register select to 1 (data register)
    writenibble(x);//send 4 msb in first (lsb cleared to 0 in writenibble)
    writenibble(x << 4);//shift left logical by 4 to send in lsb
    _delay_ms(2);
}

/*
  writenibble - Output four bits from "x" to the display
*/
void writenibble(unsigned char x)
{
    PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7) );
    //clear upper bits 4-7
    PORTD |= (x & (0xf0));
    //take msb from x by ANDing with 0xf0, and then store in PORTD 4-7
    PORTB |= (1 << PB1);//change enable line to 1
    PORTB |= (1 << PB1);//add 125ns
    PORTB &= ~(1 << PB1);//set enable line to 0
}
