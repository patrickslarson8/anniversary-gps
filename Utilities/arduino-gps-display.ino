#include <stdint.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


//GPS Settings
SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);
uint32_t timer = millis();

// Display settings
// Initialize display class
// args (display type, CS pin,# displays connected)
MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, 10, 4);
#define SPEED_TIME  25
#define PAUSE_TIME  0
#define DISPLAY_EFFECT PA_SCROLL_LEFT
#define DISPLAY_ALIGN PA_LEFT


// Game settings
// messages must end with NUL character or MD Parola overruns
const char msg_no_gps_msg[8] = { "No GPS\0" };
const char msg_gps_fix[5] = { "Fix\0" };
const char msg_boot[6] = { "Boot\0" };

void setup()
{

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);// Request updates on antenna status, comment out to keep quiet

  delay(1000);

  // Ask for GPS firmware version
  GPS.println(PMTK_Q_RELEASE);

  // Setup Displays
  P.begin();
  P.setTextBuffer(msg_boot);
  P.setTextAlignment(DISPLAY_ALIGN);
  P.setSpeed(SPEED_TIME);
  P.setPause(PAUSE_TIME);
  P.setTextEffect(DISPLAY_EFFECT, DISPLAY_EFFECT);
  P.displayReset();
}

void loop()
{
  // testing code for display and GPS integration
  // read data from the GPS in the 'main loop'
  //char c = GPS.read();

  // call display animate as often as practicable IAW library docs
  P.displayAnimate();

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    if (GPS.fix) {
      P.displayText(msg_gps_fix, PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);

    }
    else {
      P.displayText(msg_no_gps_msg, PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);

    }
  }
}