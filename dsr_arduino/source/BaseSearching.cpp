#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"

class UsFilter {
  private:
    bool usRight, usLeft;
    int rightCount, leftCount;
    int oldRight, oldLeft;
    int usNoise, usSensitivity;

  public:
    UsFilter(bool right=false, bool left=false, int sensitivity=4, int noise=5) {
      // US flags
      usRight = right;
      usLeft = left;
      // US counts
      rightCount = 0;
      leftCount = 0;
      // Keeping track of old values
      oldRight = 0;
      oldLeft = 0;
      // Allowable noise
      usNoise = noise;
      // Algorithm sensitivity
      usSensitivity = sensitivity;
    }

    int findBase(int rightDist, int leftDist) {
      //Right US
      if (usRight) {
        // Start new cycle
        if (rightCount == 0) {
          oldRight = rightDist;
        } else {
          if (abs(rightDist - oldRight) > usNoise) {
            rightCount = 1;
            oldRight = rightDist;
          } else {
            rightCount++;
            // Get average of the values
            oldRight = (oldRight*(rightCount-1) + rightDist)/rightCount;
            if (rightCount == usSensitivity) return RIGHT;
          }
        }
      }
      //Left US
      if (usLeft) {
        // Start new cycle
        if (leftCount == 0) {
          oldLeft = leftDist;
        } else {
          if (abs(leftDist - oldLeft) > usNoise) {
            leftCount = 1;
            oldLeft = leftDist;
          } else {
            leftCount++;
            // Get average of the values
            oldLeft = (oldLeft*(leftCount-1) + leftDist)/leftCount;
            if (leftCount == usSensitivity) return LEFT;
          }
        }
      }
      return NOT_FOUND;
    }
};

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
  UsFilter* filter = new UsFilter(true, false);
  int found = NOT_FOUND;
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

    // Use base algorithm
    found = filter->findBase(rightDist, -1);

    if (found != NOT_FOUND) {
      return found;
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
  UsFilter* filter = new UsFilter(true, true);
  int found = NOT_FOUND;
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

    // Use base algorithm
    found = filter->findBase(rightDist, leftDist);

    if (found != NOT_FOUND) {
      return found;
    }

    if (frontDist > 100 && frontDist != 0) {
      passed_mid = true;
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

  turn(RIGHT, 90);
  dm.updateBackUS();
  delay(2);
  dm.updateLeftUS();
  delay(2);
  dm.updateFrontUS();
  passed_mid = false;
  move(-MAX_SPEED_RIGHT/2, -MAX_SPEED_LEFT/2, 1);

  delete filter;
  filter = new UsFilter(false, true);
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

    found = filter->findBase(-1, leftDist);

    if (found != NOT_FOUND) {
      return found;
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

