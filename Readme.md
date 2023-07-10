# Anniversary GPS

This is an arduino project for my 5th anniversary. It is a game that leads the user to a specific geographic place.

## Gameplay

## Materials

### Item List

### Physical Setup

Wiring the displays:
|Arduino Pin|Display Pin|
|-|-|
|GND|GND|
|5v|Vcc|
|11|DIN|
|10|CS|
|13|CLK|

Wiring the GPS:
|Arduino Pin|GPS Pin|
|-|-|
|3.3v|Vin|
|GND|GND|
|8|TX|
|7|RX|

Wiring the switch:
Pin 2 -> Switch -> Pin 4
10k ohm resistor from GND to Pin 2

## Code

### Required Libraries

- Adafruit_GPS.h
- MD_Parola.h
- MD_MAX72xx.h

### Setup

1. You must configure the Parola library for the type of display purchased. The most common for four displays linked together is the FC16_HW. The other types are Parola, Generic, and ICStation Kit. See [this page](https://arduinoplusplus.wordpress.com/2017/04/14/parola-a-to-z-adapting-for-different-hardware/) for more information.

<code>
    #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
</code>

## Todo

- Troubleshoot str array operations
- Fix button pushes remaining rollover after 0
- Link to stl files
