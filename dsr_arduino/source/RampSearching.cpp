#pragma once
#include "DataManagement.cpp"
#include "DeviceState.cpp"
#include "Defines.h"
#include "MotorControl.cpp"
#include "PitchControl.cpp"

void ramp_searching(DataManager * dm, DeviceState* state) {
  // int sensor = 0;
  float ultrasonic_front = dm->getFrontUS();
  switch(state->current) {
  case RAMP_SEARCH:
    move(MAX_SPEED_RIGHT*0.75, MAX_SPEED_LEFT*0.75, 10);
    while(!(ultrasonic_front < 70 && ultrasonic_front > 40)) {
      dm->updateFrontUS();
      ultrasonic_front = dm->getFrontUS();
      delay(60);
    }
    while(ultrasonic_front > RAMP_DIST_X) {
      dm->updateFrontUS();
      ultrasonic_front = dm->getFrontUS();
      delay(60);
    }
    state->transition();
    break;
  case RAMP_TURN:
////      move(0,0,1);
//      move(-1*MAX_SPEED/2, MAX_SPEED/2, 4);
//      sensor = analogRead(IR_PIN);
//      // Get below 50
//      while(sensor > 170) {
//        sensor = analogRead(IR_PIN);
//        Serial.println(sensor);
////        delay(50);
//      }
//
//      while(sensor < 220) {
//        sensor = analogRead(IR_PIN);
//        Serial.println(sensor);
////        delay(50);
//      }
//      move(MAX_SPEED/2, MAX_SPEED/2, 1);
      turn(LEFT, 90, dm);
      state->transition();
    break;

  case RAMP_AHEAD:
    move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 20);
    waitFor(UP, 25, dm);
    delay(500);
    state->transition();
    break;
  }
}