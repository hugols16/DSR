#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"
#include "HeadingControl.cpp"

float approachSpeed_80 = 1.0;
float approachSpeed_target = 0.7;
float splitSpeed = 1.0;

int checkFront(float range=30, float sensitivity=1.05, float iterations=4.0) {
  DataManager dm;
  move(0,0,1);
  delay(100);
  float usFront = 0;
  float meanFront = 0;
  int countFront = 0;
  while(countFront < iterations) {
    dm.updateFrontUS();
    usFront = dm.getFrontUS();
    if(usFront > range*sensitivity) return NOT_FOUND;
    if (usFront != 0) {
      countFront++;
      meanFront += usFront/iterations;
    }
  }
  if (meanFront < range) return FRONT;
  return NOT_FOUND;
}

int checkSides(float speed, bool right=false, bool left=false, float iterations=3, int range=70, float sensitivity=1.05, int backTargetDist=999) {
  DataManager dm;
  HeadingReader hr;
  int us_right = 0;
  int us_left = 0;
  int us_back = 0;
  float mean_right = 0;
  float mean_left = 0;
  int countRight = 0, countLeft = 0;
  while((right && countRight < iterations) || (left && countLeft < iterations)) {

    dm.updateBackUS();
    us_back = dm.getBackUS();
    if (us_back > backTargetDist) return BACK;

    if (right && countRight < iterations) {
      dm.updateRightUS();
      us_right = dm.getRightUS();
      if (us_right > range*sensitivity) right = false;
      if (us_right != 0) {
        countRight++;
        mean_right += us_right/iterations;
      }
    }
    if (left && countLeft < iterations) {
      dm.updateLeftUS();
      us_left = dm.getLeftUS();
      if (us_left > range*sensitivity) left = false;
      if (us_left != 0) {
        countLeft++;
        mean_left += us_left/iterations;
      }
    }
    hr.updateForward(3, speed, speed);
  }
  if (right && mean_right < range) return RIGHT;
  if (left && mean_left < range) return LEFT;

  return NOT_FOUND;
}

int moveSearchFront(int targetDist) {
  DataManager dm;
  int found = NOT_FOUND;
  move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 20);
  HeadingReader hr;
  hr.heading = 0;
  float backDist = 0, frontDist = 0, rightDist = 0;

  // So we don't see the ramp
  while(backDist < 40) {
    dm.updateBackUS();
    backDist = dm.getBackUS();
    hr.updateForward(4, approachSpeed_80, approachSpeed_80);
  }
  // Check until 80 cm
  while(1) {

    // Use base algorithm
    found = checkSides(approachSpeed_80, false, true, 3.0, 60, 1.1, targetDist - 10.0);
    if (found == BACK) {
      break;
    }

    if (found != NOT_FOUND) return found;

  }

  while(1) {

    // Use base algorithm
    found = checkSides(approachSpeed_target, false, true, 3.0, 60, 1.1, targetDist);
    if (found == BACK) {
      break;
    }

    if (found != NOT_FOUND) return found;

  }
  found = checkFront();
  if (found == FRONT) return FRONT;
  turn(LEFT, 90 - (int) hr.heading);
  delay(50);
  return NOT_FOUND;
}

int moveOneThird() {
  DataManager dm;
  int found = NOT_FOUND;
  // move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 1);
  HeadingReader hr;
  hr.heading = 0;

  float frontDist = 0, rightDist = 0, leftDist = 0, backDist = 0;
  bool passed_mid = false;

  unsigned long t1 = micros();
  unsigned long t2 = t1;

  // Split middle
  // hr.updateForward(50, splitSpeed, splitSpeed );
  while(t2-t1 < 150000) {
    // Use base algorithm
    found = checkSides(splitSpeed-0.3, true, true, 4.0, 60, 1.05, 999);

    if (found != NOT_FOUND) {
      return found;
    }
    t2 = micros();
  }
  while(1) {
    // Use base algorithm
    found = checkSides(splitSpeed, true, true, 4.0, 60, 1.05, 999);

    if (found != NOT_FOUND) {
      return found;
    }
  }
}

void searchForBase() {
  Serial.println("Searching");
  DataManager dm;
  HeadingReader hr;
  DeviceState state;
  int found;
  found = moveSearchFront(130);
  if (found != NOT_FOUND) {
    state.transition();
    hr.updateFoundBase(found);
  }
  found = moveOneThird();
  if (found != NOT_FOUND) {
    hr.updateFoundBase(found, 95);
  }
  state.transition();
  state.transition();
  move(0,0,1);
  while(1) {}
  return;
}
