# Anniversary GPS

This is an arduino project for my 5th anniversary. It is a game that leads the user to a specific geographic place.

## Gameplay

The player discovers a mysterious box. When they push the button on the box, a message is displayed from a hidden screen! The message gives the distance (but not direction) to a destination. The player only has a limited number of times to get the distance before the box shuts down forever! How will the player find the destination? Well, triangulation is probably the best way. But having to drive around in real life and plot the destination can be kind of fun!

## Materials

### Item List

1. 3D printed pieces (https://thangs.com/designer/Pataforce8/3d-model/Arduino%20GPS%20with%20display%20box-910482)
2. Arduino Uno
3. Adafruit Ultimate GPS (https://www.amazon.com/dp/B01H1R8BK0)
4. GPS Antenna (One is built into the adafruit module but I used a seperate one for better reception) (https://www.amazon.com/dp/B07ZRBFNLH)
5. 4 LED Dot Matrix displays (https://www.amazon.com/dp/B07FFV537V)
6. A microswitch (https://www.amazon.com/dp/B07X142VGC)
7. A 10k ohm resistor
8. 9 volt battery holder (https://www.amazon.com/dp/B00LSG5AGO)
9. Various wires (https://www.amazon.com/dp/B089FZ79CS)
10. 3 small screws

### Wiring

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

### Assembly

Print out the stl files located below. I used hatchbox wood PLA and then stained it using the method in this video: https://www.youtube.com/watch?v=kuKgpi9BIiM

Program the arduino using the code in this repository. Be sure to change any game settings (like desired location) to your liking.

After the parts are printed, place all the parts into the box and secure with screws/zip ties as appropriate. I secured the dot matrix screen with a couple spots of superglue. The switch is secured with two pieces of filament and some superglue.

## Code

### Required Libraries

- TinyGPSPlus.h
- MD_Parola.h
- MD_MAX72xx.h

### Setup

1. You must configure the Parola library for the type of display purchased. The most common for four displays linked together is the FC16_HW. The other types are Parola, Generic, and ICStation Kit. See [this page](https://arduinoplusplus.wordpress.com/2017/04/14/parola-a-to-z-adapting-for-different-hardware/) for more information.

<code>
    #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
</code>

## Todo

- List types of screws used
