#pragma once
#include "DataManagement.cpp"
#include "DeviceState.cpp"
#include "Defines.h"
#include "MotorControl.cpp"
#include "PitchControl.cpp"

void ramp_moving(DataManager* dm, DeviceState* state) {
  float ramp_up_mz = dm->getMagZ();
  float new_mz = ramp_up_mz;
  float ultrasonic_front = dm->getFrontUS();
  delay(100);
  float ultrasonic_left =  dm->getLeftUS();
  delay(100);
  float heading_diff = 0;
  float min = 1000; // stupid high
  int count = 0, sensitivity = 4;
  switch(state->current) {
  case RAMP_UP:
    Serial.println("RAMP_UP");
    move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 20);
    ramp_up_mz = dm->getMagZ();
    new_mz = ramp_up_mz;
    Serial.println(new_mz);

    waitFor(DOWN, 25, dm);
    delay(10);
    state->transition();
    break;
  case RAMP_LEVEL:
    Serial.println("RAMP_LEVEL");
    move(MAX_SPEED_RIGHT/2,MAX_SPEED_LEFT/2,10);
    waitFor(DOWN, 25, dm);
    state->transition();
  case RAMP_DOWN:
    Serial.println("RAMP_DOWN");
    // Turn at the bottom
    move(MAX_SPEED_RIGHT*0.1,MAX_SPEED_LEFT*0.1,1);
      heading_diff = getHeadingDiff(dm);
      turn(RIGHT, 90 + heading_diff, dm);
    move(0,0,1);
    state->transition();
    break;
  }
}