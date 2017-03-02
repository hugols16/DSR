#include "Defines.h"

float sensorReading() {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_FRONT, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN_FRONT) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN_FRONT) == 1);
  t2 = micros();
  return (t2 - t1)/58.0;
}