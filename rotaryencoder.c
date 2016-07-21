/********************************************
*
*  Name: Arjun Narayan
*  Section: Tuesday/Thursday 2PM-3:20PM
*  Assignment: Final Project
*
********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "rotaryencoder.h"

void initialize_rotary(){
    //rotary encoder initializations:
    PCICR |= (1 << PCIE1);//enable pin change interrupts
    PCMSK1 |= (1 << PCINT9);//a0
    PCMSK1 |= (1 << PCINT10);//a1
    PORTC |= (1 << PC1);
    PORTC |= (1 << PC2);
}

void findAB(unsigned char*a, unsigned char* b){
    if((PINC & (1 << PC1)) == 0)
        *b = 1;
    else
        *b = 0;
    if((PINC & (1 << PC2)) == 0)
        *a = 1;
    else
        *a = 0;
}

void changeCount(unsigned char a, unsigned char b, int*count, unsigned char*state){
    switch(*state){//change state based on current state and a/b values
        case 1:
            if(a == 1){
                *state = 2;
                *count = *count + 1;
            }
            else if(b == 1){
                *state = 4;
                *count = *count - 1;
            }
            break;
        case 2:
            if(a == 0){
                *state = 1;
                *count = *count - 1;
            }
            else if(b == 1){
                *state = 3;
                *count = *count + 1;
            }
            break;
        case 3:
            if(a == 0){
                *state = 4;
                *count = *count + 1;
            }
            else if(b == 0){
                *state = 2;
                *count = *count - 1;
            }
            break;
        case 4:
            if(a == 1){
                *state = 3;
                *count = *count - 1;
            }
            else if(b == 0){
                *state = 1;
                *count = *count + 1;
            }
            break;
    }
}
