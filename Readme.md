# Anniversary GPS

This is an arduino project for my 5th anniversary. It is a game that leads the user to a specific geographic place.

## Gameplay

## Materials Used

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

1. Everything (Step 1 make this repo)