#include "source/DeviceState.cpp"
#include "source/sensor_reading.cpp"

// Pins
//const int TRIG_PIN = 7;
//const int ECHO_PIN = 8;

// Current (x,y) positions
volatile int x_pos = 0;
volatile int y_pos = 0;

// Expected (x,y) positions
volatile int x_exp = 0;
volatile int y_exp = 0;


// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 400.0;

void setup() {

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

void loop() {

  float dist = sensor_reading();
  if ( dist > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(dist);
    Serial.print(" cm \n");
    Serial.print(RAMP_SEARCH);
    Serial.print(" \n");
  }
  delay(60);
}
