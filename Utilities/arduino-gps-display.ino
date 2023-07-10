#include <stdint.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


//GPS Settings
SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

// Display settings
// Initialize display class
// args (display type, CS pin,# displays connected)
MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, 10, 4);
#define SPEED_TIME  25
#define PAUSE_TIME  0
#define DISPLAY_EFFECT PA_SCROLL_LEFT
#define DISPLAY_ALIGN PA_LEFT

// Switch Settings
#define POWERPIN 4
#define SWITCHPIN 2
bool button_state_debounced = false;


// Game settings
// messages must end with NUL character or MD Parola overruns
const char msg_no_gps_no_btn[16] = { "No GPS, No BTN\0" };
const char msg_gps_no_btn[13] = { "GPS, No BTN\0" };
const char msg_no_gps_btn[13] = { "No GPS, BTN\0" };
const char msg_gps_btn[10] = { "GPS, BTN\0" };
const char msg_boot[6] = { "Boot\0" };

// General stuff needed
uint32_t gps_timer = millis();

void setup()
{
  // talk to computer
  Serial.begin(115200);
  delay(5000);

  // Set up GPS
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); // Set GPS mode (RMC is most basic)
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // Set GPS update rate
  GPS.sendCommand(PGCMD_NOANTENNA);
  delay(1000);
  GPS.println(PMTK_Q_RELEASE);// Ask for GPS firmware version

  // Set up displays
  P.begin();

  // Set up switch
  pinMode(POWERPIN, OUTPUT);
  digitalWrite(POWERPIN, HIGH);
  pinMode(SWITCHPIN, INPUT);
}

void loop()
{
  // call display animate as often as practicable IAW library docs
  P.displayAnimate();

  // read latest GPS
  char c = GPS.read();

  // read pin state
  button_state_debounced = digitalRead(SWITCHPIN);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - gps_timer > 2000) {
    gps_timer = millis(); // reset the timer
    if (GPS.fix) {
      if (button_state_debounced) {
              P.displayText(msg_gps_btn, PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);
      }
      else {
              P.displayText(msg_gps_no_btn, PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);
      }
    }
    else {
      if (button_state_debounced) {
              P.displayText(msg_no_gps_btn, PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);
      }
      else {
              P.displayText(msg_no_gps_no_btn, PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);
      }
    }
  }
}