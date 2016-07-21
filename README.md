Project Description
---
This project is the code for a thermostat that imitates the climate control unit found in most buildings. It uses green and red LEDs to simulate heating and cooling. A temperature sensor is used to determine whether the system needs to heat or cool based on a set desired temperature. The thermostat can also connect to a remote device (another thermostat) and exchange information with it.

The thermostat is built with an Arduino Uno, and Avr-gcc compiler is used to compile C on the Arduino.


Project Components
---

![Alt text](/Images/Diagram1.png?raw=true "Project Components")


DS1631
---

The DS1631 temperature sensor communicates with the micro controller using I2C. High and low temperatures are set via a rotary encoder and buttons to switch between control of both settings.


![Alt text](/Images/Diagram2.png?raw=true)


Rs-232
---
The thermostat can communicate its local temperature to other thermostats with a serial interface that uses an RS-232 link to transmit data. It uses a MAX232 tri-state buffer to allow the USB interface to upload code.

![Alt text](/Images/Diagram3.png?raw=true)


Rotary Encoder
---

A rotary encoder is used to modify the high and low temperature values. It uses a state machine and interrupts to accurately modify values.

![Alt text](/Images/Diagram4.png?raw=true)


LCD Screen
---

The LCD shield uses a parallel interface and can transfer 4 bits at a time. 

![Alt text](/Images/Diagram5.png?raw=true)
