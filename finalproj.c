/********************************************
*
*  Name: Arjun Narayan
*  Section: Tuesday/Thursday 2PM-3:20PM
*  Assignment: Final Project
*
********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include "ds1631.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void updateScreen();
void updateTop();
void updateLED();

volatile unsigned char buttonState;
volatile unsigned char change = 0;
volatile unsigned char low = 60;
volatile unsigned char high = 80;
volatile unsigned char state = 1;
volatile int count = 0;
volatile int tempF = 0;
volatile int oldTempF = 0;

volatile unsigned char receivedDataBuffer[4];
volatile int bufferCount = 0;
volatile unsigned char bufferValidFlag = 0;
volatile int remoteVal = 0;

#define FOSC 16000000 // Clock frequency
#define BAUD 9600 // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0

#include "rotaryencoder.h"
#include "serial.h"

int main(void)
{
    init_lcd();
    writecommand(0x01);
    updateScreen();

    DDRD |= (1 << PD2);//output for green LED
    DDRD |= (1 << PD3);//output for red LED

    //button pullup resistors:
    PORTB |= (1 << PB3) | (1 << PB4);

    //rotary encoder initializations:
    initialize_rotary();
    sei();

    PORTC |= (1 << PC4) | (1 << PC5);//i2c pullup resistors

    //enable line
    PORTC |= (1 << PC3);
    DDRC |= (1 << PC3); //set as output

    //DS1631
    ds1631_init();
    ds1631_conv();
    unsigned char temp[2];
    //serial communication
    init_serial();

    while (1) {
        PORTC &= ~(1 << PC3);//set enable to 0
        ds1631_temp(temp);
        int tempCelsius = temp[0];
        //convert from celsius to fahrenheit:
        if(temp[1] == 0){
            tempF = (tempCelsius*9)/5 + 32;
        }
        else{
            tempF = tempCelsius * 10 + 5;
            tempF = (tempF*9)/5 + 320;
            tempF /= 10;
        }

        if(oldTempF != tempF){//if temperature changes, update + transmit data
            //update temp
            updateTop();
            char temp[5];
            snprintf(temp, 5, "%d", tempF);
            transmitData(temp, &tempF);
        }

        oldTempF = tempF;//update old temp

        if((PINB & (1 << PB3)) == 0){//if high button is pressed
            buttonState = 1;//high
        }
        if((PINB & (1 << PB4)) == 0){//if low button is pressed
            buttonState = 0;//low
        }

        if(bufferValidFlag == 1){//valid data received
            //convert to int stored inside remoteVal
            int hundreds = (receivedDataBuffer[1]-0x30)*100;
            int tens = (receivedDataBuffer[2]-0x30)*10;
            int ones = receivedDataBuffer[3]-0x30;
            remoteVal = hundreds + tens + ones;
            if(receivedDataBuffer[0] == '-'){
                remoteVal = 0 - remoteVal;
            }
            updateTop();
            bufferValidFlag = 0;//reset
        }

        if(change == 1){
            updateScreen();
            change = 0;//reset
        }

        updateLED();
    }
    return 0;   /* never reached */
}

void updateTop(){//called if valid data received from remote
    moveto(0x00);
    char top[16];
    snprintf(top, 16, "Temp:%d Rmt:%d", tempF, remoteVal);
    stringout(top);
}

void updateScreen(){
    moveto(0x40);//move to second line of LCD
    char vars[16];
    if(buttonState){//high
        snprintf(vars, 16, "Low:%d HIGH:%d", low, high);
    }
    else{//low
        snprintf(vars, 16, "LOW:%d High:%d", low, high);
    }
    stringout(vars);
}

void updateLED(){
    if(tempF < low){//turn on red light
        PORTD |= (1 << PD3);
    }
    else if(tempF >= low){
        PORTD &= ~(1 << PD3);
    }
    if(tempF > high){//turn on green light
        PORTD |= (1 << PD2);
    }
    else if(tempF <= high){
        PORTD &= ~(1 << PD2);
    }
}

ISR(USART_RX_vect){
      char x = UDR0;
      if(x == '-' || x == '+'){
          receivedDataBuffer[0] = x;
          bufferCount = 1;
          bufferValidFlag = 0;//incomplete data
      }
      else if(isDigit(x)){
          receivedDataBuffer[bufferCount] = x;
          bufferCount++;
      }
      else{//reset if invalid data comes through
          bufferCount = 0;
      }

      if(bufferCount == 4){
          bufferValidFlag = 1;//valid data
          bufferCount = 0;
      }
  }

  ISR(PCINT1_vect){
      unsigned char a = 0;
      unsigned char b = 0;
      findAB(&a, &b);
      //^find a and b values
      //set count to which ever value must be changed:
      if(buttonState == 1){//high
          count = high;
      }
      else if(buttonState == 0){//low
          count = low;
      }
      //change the value:
      changeCount(a, b, &count, &state);
      //update low/high and bound it
      if(buttonState == 1){//high
          high = count;
          if(high <= low){
              high = low+1;
          }
      }
      else if(buttonState == 0){//low
          low = count;
          if(low >= high){
              low = high-1;
          }
      }
      change = 1;
  }
