#include "source/DeviceState.cpp"
// Pins
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;

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


float sensor_reading() {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  return (t2 - t1)/58.0;
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
