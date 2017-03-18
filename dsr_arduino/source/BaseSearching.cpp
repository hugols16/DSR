#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"

void driveToBase(int found){
  Serial.println("FOUND!");
  DeviceState state;
  if(found != FORWARD){
    delay(200);
    turn(found, 90);
  }
  move(0,0,1);
  delay(200);
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 1);
  while(1);
  return;
}

int moveSearchFront(int targetDist) {
  DataManager dm;
  move(-MAX_SPEED_RIGHT/2, -MAX_SPEED_LEFT/2, 20);
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

    if (backDist < 40 && backDist != 0) {
      Serial.println("Forward");
      return FORWARD;
    }
    delay(40);
  }
  turn(LEFT, 90);
  Serial.println("Not Found");
  return NOT_FOUND;
}

int moveOneThird() {
  DataManager dm;
  move(-MAX_SPEED_RIGHT/2, -MAX_SPEED_LEFT/2, 1);
  float frontDist = 0, rightDist = 0, leftDist = 0, backDist = 0;
  bool passed_mid = false;

  // Split middle
  while(frontDist < 100 or (passed_mid && backDist > 20)) {
    dm.updateBackUS();
    delay(2);
    dm.updateFrontUS();
    delay(2);
    dm.updateRightUS();
    delay(2);
    dm.updateLeftUS();

    backDist = dm.getBackUS();
    frontDist = dm.getFrontUS();
    rightDist = dm.getRightUS();
    leftDist = dm.getLeftUS();

    if (frontDist > 100 && frontDist != 0) {
      passed_mid = true;
    }
    if (rightDist < 60 && rightDist != 0) {
      Serial.println("Right");
      return LEFT;
    }

    if (leftDist < 60 && leftDist != 0) {
      Serial.println("Left");
      return RIGHT;
    }
    if (backDist < 40 && backDist != 0 && !passed_mid) {
      Serial.println("Forward");
      return FORWARD;
    }
    delay(40);
  }

  Serial.println(backDist);
  turn(RIGHT, 90);
  dm.updateBackUS();
  backDist = dm.getBackUS();
  move(-MAX_SPEED_RIGHT/2, -MAX_SPEED_LEFT/2, 1);
  Serial.println(backDist);
  // Move parallel to the wall
  while(!(backDist < 70 && backDist > 40)) {
    dm.updateBackUS();
    backDist = dm.getBackUS();
    delay(60);
  }
  while(backDist < 30) {
    dm.updateBackUS();
    backDist = dm.getBackUS();
    delay(60);
  }

  Serial.println("Turning");
  turn(RIGHT, 90);
  dm.updateBackUS();
  delay(2);
  dm.updateLeftUS();
  delay(2);
  dm.updateFrontUS();
  passed_mid = false;
  move(-MAX_SPEED_RIGHT/2, -MAX_SPEED_LEFT/2, 1);

  // Check again for edge cases
  while(frontDist < 100 or (passed_mid && backDist > 20)) {
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
    if (backDist < 40 && backDist != 0 && !passed_mid) {
      Serial.println("Forward");
      return FORWARD;
    }
    delay(50);
  }
  return NOT_FOUND;
}

void searchForBase() {
  DataManager dm;
  DeviceState state;
  int found = moveSearchFront(110);
  if (found != NOT_FOUND) {
    state.transition();
    driveToBase(found);
  }
  found = moveOneThird();
  if (found != NOT_FOUND) {
    driveToBase(found);
  }
  state.transition();
  state.transition();
  move(0,0,1);
  return;
}

