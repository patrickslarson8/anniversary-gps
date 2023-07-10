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
#define DEBOUNCE_TIME 2000
bool button_state = false;
bool button_state_debounced = false;

// Game settings
const float dest_latitude = 0.0f;
const float desst_longitude = 0.0f;
uint8_t btn_pushes_remaining = 11; // add an extra to display the welcome message

// Messages (Must end with null character)
char* str_buffer = (char *) malloc(1);
const char first_time_msg[162] = "Happy Anniversary my love! When the button is pressed, the distance to your anniversary getaway will be displayed. You have ten button presses to it. Good luck!\0";
const char game_over_msg[54] = "Oh no! You are out of tries. No anniversary for Meg.\0"; //todo
const char game_msg[27] = "Button pushes remaining: \0"; //todo
const char msg_no_gps_msg[59] = "Unable to acquire GPS. Please move outside and try again.\0";

// General stuff needed
uint32_t gps_timer = millis();
uint32_t btn_timer = millis();
float distance_to_dest = 0.0f;

float calculate_distance(nmea_float_t, nmea_float_t)
{
  //TODO
}

void display(const char* msg)
{
  P.displayText(msg, PA_LEFT, SPEED_TIME, PAUSE_TIME, DISPLAY_EFFECT, DISPLAY_EFFECT);
}

char* display(char* buffer, const char* new_msg, uint8_t msg_size, uint8_t num_to_append)
{
  // set up new memory space
  free(buffer);
  buffer = (char *) malloc(msg_size + 1);
  Serial.println("spaceFreed");

  // strip null character and add number
  strcpy(buffer, new_msg);
  char num_as_char = (char) num_to_append;
  buffer[msg_size] = num_as_char;
  buffer[msg_size + 1] = '\0';
  Serial.println("msgAppended");
  
  // Display new array
  display(buffer);

  // return pointer to buffer so main program can keep track
  // of things
  return buffer;
}

// These are all the things that must be done
// as often as possible to keep things running
// smoothly
void background_tasks()
{
  // call display animate as often as practicable IAW library docs
  P.displayAnimate();

  // read latest GPS
  char c = GPS.read();

  // read pin state
  button_state = digitalRead(SWITCHPIN);

  // routine to debounce button
  if (button_state){
    if ((millis() - btn_timer) < DEBOUNCE_TIME) {
      button_state_debounced = false;
    }
    else {
      button_state_debounced = true;
      Serial.println("btn");
      btn_timer = millis();
    }
  }

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  return;
}

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

    // psuedo code for game loop
  background_tasks();

  if (button_state_debounced)
  {
    button_state_debounced = false; // set to false to prevent counting twice
    Serial.println(btn_pushes_remaining);
    switch (btn_pushes_remaining--) {
    case 10: 
      display(first_time_msg);
      Serial.println(first_time_msg);
      break;
    case 0: 
      display(game_over_msg);
      Serial.println(game_over_msg);
      break;
    default:
      str_buffer = display(str_buffer, game_msg, 27, btn_pushes_remaining);
      break;
    }
  } 
}