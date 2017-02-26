#include "source/Defines.h"
#include "source/DeviceState.cpp"
#include "source/SensorReading.cpp"

// Pins
//const int TRIG_PIN = 7;
//const int ECHO_PIN = 8;

// For testing
volatile float dist = 0;

// Current (x,y) positions
volatile int x_pos = 0;
volatile int y_pos = 0;

// Expected (x,y) positions
volatile int x_exp = 0;
volatile int y_exp = 0;

// Create Global State Machine
DeviceState* state;

//Might want to put the following in the Defines
// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 400.0;

// Starting x distance from the ramp (exact distance to middle)
const unsigned int RAMP_DIST_X = 35.0;

// Starting y distance from the ramp (exact distance to bottom)
const unsigned int RAMP_DIST_Y = 35.0;

void setup() {

  // Create State Machine
  state = new DeviceState();
  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);

  // Move to READY
  state->transition();
  // Move to RAMP_SEARCH (if we don't use a button)
  state->transition();
}

void ramp_searching() {
  //ultrasonic_left = DataManager.get_us_left()
  float ultrasonic_left = dist;
  if (ultrasonic_left < (RAMP_DIST_Y + 10) || x_pos >= RAMP_DIST_X) {
    // rotate 90 degrees to the left
    // drive to the ramp BY RAMP_DIST_X centimeters
    state->transition();
  } else {
    //x_exp++;
    //motor_movement
  }
}
void loop() {
  // Update DataManager
  dist = sensorReading();
  Serial.print(state->current);
  Serial.print(" \n");
  switch(state->current) {
    case RAMP_SEARCH:
      ramp_searching();
      break;
    case RAMP_UP:
      break;
    case RAMP_LEVEL:
      break;
    case RAMP_DOWN:
      break;
    case SEARCHING:
      break;
  }
  if ( dist > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(dist);
    Serial.print(" cm \n");
  }
  delay(60);
}
