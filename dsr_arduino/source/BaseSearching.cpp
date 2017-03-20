#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"
#include "HeadingControl.cpp"

void driveToBase(int found){
  HeadingReader hr;
  Serial.println("FOUND!");
  DeviceState state;
  if(found != FORWARD){
    delay(200);
    turn(found, 90);
  }
  move(0,0,1);
  delay(200);
  hr.heading = 0;
  while(1) {
    hr.update(100);
  };
  return;
}

int moveSearchFront(int targetDist) {
  DataManager dm;
  HeadingReader hr;

  hr.heading = 0;

  float backDist = 0, frontDist = 0, rightDist = 0;

  while(frontDist < targetDist) {
    dm.updateBackUS();
    delay(2);
    dm.updateFrontUS();
    delay(2);
    dm.updateRightUS();

    backDist = dm.getBackUS();
    frontDist = dm.getFrontUS();
    rightDist = dm.getRightUS();

    if (rightDist < 60 && rightDist != 0) {
      Serial.println("Right");
      return LEFT;
    }

    // if (backDist < 40 && backDist != 0) {
    //   Serial.println("Forward");
    //   return FORWARD;
    // }
    hr.update(5);
  }
  turn(LEFT, 90 - (int) hr.heading);
  Serial.println("Not Found");
  return NOT_FOUND;
}

int moveOneThird() {
  DataManager dm;
  HeadingReader hr;

  hr.heading = 0;

  float frontDist = 0, rightDist = 0, leftDist = 0, backDist = 0;
  bool passed_mid = false;

  for(int i = 0; i < 10; i++) {
    delay(5);
    dm.updateRightUS();
    delay(5);
    dm.updateLeftUS();
    delay(40);
  }
  
  // Split middle
  while(frontDist < 100 || (passed_mid && backDist > 20)) {
    dm.updateBackUS();
    delay(5);
    dm.updateFrontUS();
    delay(5);
    dm.updateRightUS();
    delay(5);
    dm.updateLeftUS();

    backDist = dm.getBackUS();
    frontDist = dm.getFrontUS();
    rightDist = dm.getRightUS();
    leftDist = dm.getLeftUS();

    Serial.println(leftDist);

    if (frontDist > 100 && frontDist != 0) {
      passed_mid = true;
    }
    if (rightDist < 60 && rightDist != 0) {
      Serial.println("Right");
      return LEFT;
    }

    if (leftDist < 60 && leftDist != 0) {
      move(0,0,1);
      while(1){
        Serial.println("Left");
        Serial.println(leftDist);
        dm.updateLeftUS();
        leftDist = dm.getLeftUS();
      }
      return RIGHT;
    }
    // if (backDist < 40 && backDist != 0 && !passed_mid) {
    //   Serial.println("Forward");
    //   return FORWARD;
    // }
    hr.update(5);
  }

  Serial.println(backDist);
  turn(RIGHT, 90 + (int) hr.heading);

  dm.updateBackUS();
  backDist = dm.getBackUS();
  hr.heading = 0;
  Serial.println(backDist);
  // Move parallel to the wall
  while(!(backDist < 70 && backDist > 40)) {
    hr.update(6);
    dm.updateBackUS();
    backDist = dm.getBackUS();
  }
  while(backDist > 30) {
    dm.updateBackUS();
    backDist = dm.getBackUS();
    hr.update(6);
  }

  turn(RIGHT, 90 - (int) hr.heading);

  dm.updateBackUS();
  delay(2);
  dm.updateLeftUS();
  delay(2);
  dm.updateFrontUS();
  passed_mid = false;

  hr.heading = 0;

  // Check again for edge cases
  while(frontDist < 100 || (passed_mid && backDist > 20)) {
    dm.updateBackUS();
    delay(2);
    dm.updateFrontUS();
    delay(2);
    dm.updateLeftUS();

    backDist = dm.getBackUS();
    frontDist = dm.getFrontUS();
    leftDist = dm.getLeftUS();

    if (frontDist > 100 && frontDist != 0) {
      passed_mid = true;
    }

    if (leftDist < 60 && leftDist != 0) {
      Serial.println("Left");
      return RIGHT;
    }
    // if (backDist < 40 && backDist != 0 && !passed_mid) {
    //   Serial.println("Forward");
    //   return FORWARD;
    // }
    hr.update(5);
  }
  return NOT_FOUND;
}

void searchForBase() {
  DataManager dm;
  DeviceState state;
  int found = moveSearchFront(100);
  // if (found != NOT_FOUND) {
  //   state.transition();
  //   driveToBase(found);
  // }
  found = moveOneThird();
  if (found != NOT_FOUND) {
    driveToBase(found);
  }
  state.transition();
  state.transition();
  move(0,0,1);
  while(1) {}
  return;
}
