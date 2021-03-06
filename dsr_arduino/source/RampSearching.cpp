#pragma once
#include "DataManagement.cpp"
#include "DeviceState.cpp"
#include "Defines.h"
#include "MotorControl.cpp"
#include "PitchControl.cpp"
#include "HeadingControl.cpp"

float rampSearchSpeed = 1.0;

void ramp_searching() {
  DeviceState state;
  DataManager dm;

  HeadingReader hr;
  hr.heading = 0;

  float ultrasonic_front = dm.getFrontUS();

  switch(state.current) {
  case RAMP_SEARCH:
    hr.updateForward(100, rampSearchSpeed, rampSearchSpeed);
    while(ultrasonic_front == 0 || ultrasonic_front > RAMP_DIST_X) {
      dm.updateFrontUS();
      ultrasonic_front = dm.getFrontUS();
      hr.updateForward(4, rampSearchSpeed, rampSearchSpeed);
    }
    state.transition();
    break;

  case RAMP_TURN:
      turn(LEFT, 90 -  (int) hr.heading, 0.4);
      state.transition();
    break;

  case RAMP_AHEAD:
    move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 10);
    waitFor(UP, 25);
    delay(500);
    state.transition();
    break;
  }
}
