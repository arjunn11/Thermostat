/********************************************
*
*  Name: Arjun Narayan
*  Section: Tuesday/Thursday 2PM-3:20PM
*  Assignment: Final Project
*
********************************************/

#include <avr/io.h>
#include "serial.h"
#define FOSC 16000000 // Clock frequency
#define BAUD 9600 // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0

void init_serial(){
    UBRR0 = MYUBRR; //Set baud rate
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);
    //Enable RX (receiver) and TX (transmitter)
    UCSR0C = (3 << UCSZ00); // Async., no parity, 1 stop bit, 8 data bits
    //Enable receiver interrupts:
    UCSR0B |= (1 << RXCIE0);
}

void tx_char(unsigned char ch)
{
    // Wait for transmitter data register empty
    while ((UCSR0A & (1<<UDRE0)) == 0) {}
    UDR0 = ch;
}

void transmitData(char* str, int* tempF){
    if(*tempF >= 0){
        tx_char('+');
    }
    else if(*tempF < 0){
        tx_char('-');
        *tempF = 0 - *tempF;
    }
    if(abs(*tempF) < 100){
        tx_char('0');
    }
    int i;
    //send the remainder of the digits in tempF
    for(i = 0; str[i] != '\0'; i++){
       tx_char(str[i]);
    }
}

unsigned char isDigit(char c){//returns if a character is a digit
    if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5'
    || c == '6' || c == '7' || c == '8' || c == '9'){
        return 1;
    }
    return 0;
}
