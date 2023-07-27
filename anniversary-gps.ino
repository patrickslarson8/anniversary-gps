#include <stdint.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


//GPS Settings
SoftwareSerial ss(8, 7);
TinyGPSPlus gps;

// Display settings
// Initialize display class
// args (display type, CS pin,# displays connected)
MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, 10, 4);
#define SPEED_TIME  100
#define PAUSE_TIME  0
#define DISPLAY_EFFECT PA_SCROLL_LEFT
#define DISPLAY_ALIGN PA_LEFT
bool display_finished = true;

// Switch Settings
#define POWERPIN 4
#define SWITCHPIN 2
#define DEBOUNCE_TIME 2000
bool button_state = false;
bool button_state_debounced = false;

// Game settings
//set to Steven's River Cabin see https://www.google.com/maps/place/Steven%E2%80%99s+River+cabin/@48.1029156,-121.9454161,17z
static const double CABIN_LAT = 48.102915, CABIN_LON = -121.945416;
#define max_num_btn_pushes 12
uint8_t btn_pushes_remaining = max_num_btn_pushes;

// Messages
const String first_time_msg = "Happy Anniversary! When the button is pressed, the distance to your anniversary getaway will be displayed. You have ten tries. Good luck!";
const String game_over_msg = "Oh no! You are out of tries. No anniversary for Meg.";
const String game_msg_btn = "Button pushes remaining:  ";
const String game_msg_dist = "Distance to anniversary: ";
const String units = " mi. ";
const String no_gps_msg = "Unable to acquire GPS. Please move outside and try again.";
String temp;

// General stuff needed
uint32_t gps_timer = millis();
uint32_t btn_timer = millis();
#define km_to_mi_conversion 0.621371

// Pushes a new message to display, but only if the display is finished
void display(const String &msg)
{
  if (display_finished){
    P.displayText(msg.c_str(), PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);
  }
}

void display(const String &msg, double distance, uint8_t num)
{
  temp = msg;

  temp.concat(distance);
  temp.concat(units);
  temp.concat(game_msg_btn);
  temp.concat(num);

  Serial.println(temp);

  P.displayText(temp.c_str(), PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);
}

// These are all the things that must be done
// as often as possible to keep things running
// smoothly
void background_tasks()
{
  // call display animate as often as practicable IAW library docs
  display_finished = P.displayAnimate();

  // read latest GPS
  if (ss.available()){
      gps.encode(ss.read());
  }

  button_state = digitalRead(SWITCHPIN);

  // routine to debounce button
  if (button_state){
    if ((millis() - btn_timer) < DEBOUNCE_TIME) {
      button_state_debounced = false;
    }
    else {
      button_state_debounced = true;
      btn_timer = millis();
    }
  }

  return;
}

void setup()
{
  // talk to computer
  Serial.begin(115200);
  delay(5000);

  // Set up GPS
  ss.begin(9600);
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); // Set GPS mode (RMC is most basic)
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // Set GPS update rate
  // GPS.sendCommand(PGCMD_NOANTENNA);
  // delay(1000);
  // GPS.println(PMTK_Q_RELEASE);// Ask for GPS firmware version

  // Set up displays
  P.begin();

  // Set up switch
  pinMode(POWERPIN, OUTPUT);
  digitalWrite(POWERPIN, HIGH);
  pinMode(SWITCHPIN, INPUT);
}

void loop()
{
  background_tasks();

  if (button_state_debounced && display_finished){
    if (!gps.location.isValid()){
    display(no_gps_msg);
    }
    else{
      button_state_debounced = false; // set to false to prevent counting twice
      Serial.println(btn_pushes_remaining);
      switch (btn_pushes_remaining) {

      case max_num_btn_pushes: 
        display(first_time_msg);
        btn_pushes_remaining--;
        break;

      case 0: 
        display(game_over_msg);
        break;

      default:
        double distance = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), CABIN_LAT, CABIN_LON) / 1000;
        distance = distance * km_to_mi_conversion;

        btn_pushes_remaining--;
        display(game_msg_dist, distance, btn_pushes_remaining);
        break;
      }
    }
  } 
}