#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"

void moveSearchFront(int moveDist) {
  DataManager dm;
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 20);

  float backDist;
  int countBack = 0, countFront = 0;

  while(countBack < 4) {
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
      while(1) {}
    }

    delay(3);
  }
  move(0, 0, 1);
}

int moveSearchFrontAndSides (int moveDist) {
  DataManager dm;
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 20);

  float backDist, frontDist, leftDist, rightDist;
  int countBack = 0, countFront = 0;

  while(countBack < 4) {
    dm.updateBackUS();
    delay(2);
    dm.updateFrontUS();
    delay(2);
    dm.updateLeftUS();
    delay(2);
    dm.updateRightUS();

    backDist = dm.getBackUS();
    frontDist = dm.getFrontUS();
    leftDist = dm.getLeftUS();
    rightDist = dm.getRightUS();

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
      while(1) {}
    }



    delay(3);
  }

  move(0, 0, 1);

  return RIGHT;
}

void searchForBase() {

  DataManager dm;
  moveSearchFront(100);

  turn(RIGHT, 90);

  int turnDir = moveSearchFrontAndSides(150);

  turn(turnDir, 90);

  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 3);

  delay(5000);

  move(0, 0, 1);
}
