#include <stdint.h>
#include <Adafruit_GPS.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);

#define GPSECHO false

uint32_t timer = millis(); //todo: see if we can shrink this down
uint8_t btn_pushes_remaining = 10;

const int btn_pin = 2;
const float dest_latitude = 0.0f;
const float desst_longitude = 0.0f;
int btn_state = 0;
float distance_to_dest = 0.0f;

const char* first_time_msg = "Happy Anniversary my love, let's play a game. When the button is pressed, the distance to your anniversary getaway will be displayed. You have ten button presses to find me. Good luck!";
const char* game_over_msg = "Oh no! You are out of tries. No anniversary for Meg."; //todo
const char* game_msg = "Button pushes remaining: "; //todo
const char* no_gps_msg = "Unable to acquire GPS. Please move outside and try again.";

void display(const char*)
{
  // TODO
}

void display(const char*, uint8_t)
{

}

float calculate_distance(nmea_float_t, nmea_float_t)
{
  //TODO
}

void setup()
{
  pinMode(btn_pin, INPUT);

  while (!Serial);  // uncomment to have the sketch wait until Serial is ready
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic parsing test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

}

void loop()
{
	// wait for button push
    // first button push, display welcome screen
    // subsequent button pushes
        // get gps position
            // error message
        // calculate distance to destination
        // display distance to destination
        // display number of tries remaining
        // wait for next button push
    // max number of button pushes
        // keep showing or say game over?? leaning towards game over.

    btn_state = digitalRead(btn_pin);
    if (btn_state == 1)
    {
      if (GPS.fix)
      {
        btn_pushes_remaining++;
        switch(btn_pushes_remaining)
        {
          case 0: display(game_over_msg);
          case 10: display(first_time_msg);
          default: 
            distance_to_dest = calculate_distance(GPS.latitude, GPS.longitude);
            display(game_msg, btn_pushes_remaining);
        }
        btn_pushes_remaining--;
      }
      else
      {
        display(no_gps_msg);
      }
    }

// read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);
    }
  }
}