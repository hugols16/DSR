#pragma once
#include "DataManagement.cpp"
#include "DeviceState.cpp"
#include "Defines.h"
#include "MotorControl.cpp"
#include "PitchControl.cpp"

void ramp_moving() {
  DeviceState state;
  DataManager dm;

  float heading_diff = 0;
  delay(100);

  switch(state.current) {
  case RAMP_UP:
    Serial.println("RAMP_UP");
    move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 1);
    waitFor(DOWN, 25);
    delay(10);
    state.transition();
    break;
  case RAMP_LEVEL:
    Serial.println("RAMP_LEVEL");
    move(MAX_SPEED_RIGHT / 2, MAX_SPEED_LEFT / 2, 10);
    waitFor(DOWN, 25);
    state.transition();
  case RAMP_DOWN:
    Serial.println("RAMP_DOWN");
    move(MAX_SPEED_RIGHT * 0.2, MAX_SPEED_LEFT * -0.1, 1);
    heading_diff = getHeadingDiff();
    turn(LEFT, 85 - heading_diff);
    state.transition();
    break;
  }
}
