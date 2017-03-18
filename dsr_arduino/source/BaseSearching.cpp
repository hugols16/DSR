#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"

void moveSearchFront(int targetDist) {
  DataManager dm;
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 20);

  float backDist, frontDist;
  int countBack = 0, countFront = 0;

  while(countFront < 4) {
    dm.updateBackUS();
    delay(2);
    dm.updateFrontUS();

    backDist = dm.getBackUS();
    frontDist = dm.getFrontUS();

    if(frontDist > targetDist) {
      countFront++;
    } else {
      countFront = 0;
    }

    if(backDist < 40) {
      countBack++;
    } else {
      countBack = 0;
    }

    if(countBack > 8) {
      move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 1);
      delay(4000);
      move(0, 0, 1);
      Serial.print("Ending with back");
      while(1) {};
    }

    Serial.print("frontDist: ");
    Serial.print(frontDist);
    Serial.print("\n");

    delay(3);
  }
  move(0, 0, 1);
  Serial.print("Ending with front");
  return;
}

float ratio = 0.6;
int moveSearchFrontAndSides (int targetDist) {
  DataManager dm;
  move(-MAX_SPEED_RIGHT*ratio, -MAX_SPEED_LEFT*ratio, 20);

  float backDist, frontDist, leftDist, rightDist;
  int countBack = 0, countFront = 0, countLeft = 0, countRight = 0;

  while(countBack < 4) {
    // dm.updateBackUS();
    // delay(2);
    dm.updateFrontUS();
    delay(1);
    dm.updateLeftUS();
    delay(1);
    dm.updateRightUS();

    // backDist = dm.getBackUS();
    frontDist = dm.getFrontUS();
    leftDist = dm.getLeftUS();
    rightDist = dm.getRightUS();

    // if(frontDist > targetDist) {
    //   countFront++;
    // } else {
    //   countFront = 0;
    // }

    if(leftDist < 75) {
      countLeft++;
    } else {
      countLeft = 0;
    }

    if(rightDist < 110 && frontDist > 150) {
      countRight++;
    } else {
      countRight = 0;
    }

    // if(countFront > 8) {
    //   move(0, 0, 1);
    // }

    if(countRight > 10) {
      move(0, 0, 1);
      Serial.println("LEFT");
      return LEFT;
    }

    if(countLeft > 10) {
      move(0, 0, 1);
      Serial.println("RIGHT");
      return RIGHT;
    }

    // if(backDist < 40) {
    //   countBack++;
    // } else {
    //   countBack = 0;
    // }

    // if(countBack > 8) {
    //   move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 1);
    //   delay(4000);
    //   move(0, 0, 1);
    //   while(1) {}
    // }

    Serial.print("r: ");
    Serial.print(leftDist);
    Serial.print("   l: ");
    Serial.print(rightDist);
    Serial.print("\n");

    delay(25);
  }

  move(0, 0, 1);
}

void searchForBase() {

  DataManager dm;
  // moveSearchFront(115);
  //
  // turn(LEFT, 90);

  // for(int i = 0; i < 60; i++) {
  //   dm.updateLeftUS();
  //   delay(16);
  //   dm.updateRightUS();
  //   delay(16);
  // }

  // move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 10);
  //
  // Serial.println("Done");
  //
  for(int i = 0; i < 20; i++) {
    dm.updateLeftUS();
    delay(1);
    dm.updateRightUS();
    delay(16);
    Serial.print("l: ");
    Serial.print(dm.getRightUS());
    Serial.print("   r: ");
    Serial.print(dm.getLeftUS());
    Serial.print("\n");
  }

  int turnDir = moveSearchFrontAndSides(200);

  // Serial.println("Done");
  // Serial.print("\n");
  //
  // for(int i = 0; i < 60; i++) {
  //   dm.updateLeftUS();
  //   delay(1);
  //   dm.updateRightUS();
  //   delay(16);
  //   Serial.print("l: ");
  //   Serial.print(dm.getRightUS());
  //   Serial.print("   r: ");
  //   Serial.print(dm.getLeftUS());
  //   Serial.print("\n");
  // }

  //
  // turn(turnDir, 90);
  //
  // move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 3);
  //
  // delay(5000);
  //
  // move(0, 0, 1);
}
