#include "Defines.h"

NewPing frontUS(TRIG_PIN_FRONT, ECHO_PIN_FRONT, 150);

float sensorReadingFront() {
//   unsigned long t1;
//   unsigned long t2;
//   unsigned long pulse_width;

//   // Hold the trigger pin high for at least 10 us
//   digitalWrite(TRIG_PIN_FRONT, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN_FRONT, LOW);

//   // Wait for pulse on echo pin
//   while ( digitalRead(ECHO_PIN_FRONT) == 0 );

//   // Measure how long the echo pin was held high (pulse width)
//   // Note: the micros() counter will overflow after ~70 min
//   t1 = micros();
//   while ( digitalRead(ECHO_PIN_FRONT) == 1);
//   t2 = micros();
//   return (t2 - t1)/58.0;
  return frontUS.ping_cm();
}

float sensorReadingRight() {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_RIGHT, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN_RIGHT) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN_RIGHT) == 1);
  t2 = micros();
  return (t2 - t1)/58.0;
}

float sensorReadingLeft() {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_LEFT, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN_LEFT) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN_LEFT) == 1);
  t2 = micros();
  return (t2 - t1)/58.0;
}

// float sensorReadingBack() {
//   unsigned long t1;
//   unsigned long t2;
//   unsigned long pulse_width;

//   // Hold the trigger pin high for at least 10 us
//   digitalWrite(TRIG_PIN_BACK, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN_BACK, LOW);

//   // Wait for pulse on echo pin
//   while ( digitalRead(ECHO_PIN_BACK) == 0 );

//   // Measure how long the echo pin was held high (pulse width)
//   // Note: the micros() counter will overflow after ~70 min
//   t1 = micros();
//   while ( digitalRead(ECHO_PIN_BACK) == 1);
//   t2 = micros();
//   return (t2 - t1)/58.0;
// }
