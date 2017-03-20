#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"
#include "HeadingControl.cpp"

class UsFilter {
  private:
    bool usRight, usLeft;
    int rightCount, leftCount;
    int oldRight, oldLeft;
    int usNoise, usSensitivity, usMaxDist;

  public:
    UsFilter(bool right=false, bool left=false, int sensitivity=3, int noise=2, int max_dist=50) {
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

      // Max Allowable Distance
      usMaxDist = max_dist;
    }

    int findBase(int rightDist, int leftDist) {
      //Right US
      if (usRight && rightDist && rightDist < usMaxDist) {
        // Start new cycle
        if (rightCount == 0) {
          oldRight = rightDist;
          rightCount++;
        } else {
          if (abs(rightDist - oldRight) > usNoise) {
            rightCount = 1;
            oldRight = rightDist;
          } else {
            rightCount++;
            // Get average of the values
            oldRight = (oldRight*(rightCount-1) + rightDist)/rightCount;
            if (rightCount == usSensitivity) {
              Serial.println(rightCount);
              Serial.println("FOUND RIGHT");
              return LEFT;
            }
          }
        }
      } else {
        rightCount = 0;
      }
      //Left US
      if (usLeft && leftDist && leftDist < usMaxDist) {
        // Start new cycle
        if (leftCount == 0) {
          oldLeft = leftDist;
          leftCount++;
        } else {
          if (abs(leftDist - oldLeft) > usNoise) {
            leftCount = 1;
            oldLeft = leftDist;
          } else {
            leftCount++;
            // Get average of the values
            oldLeft = (oldLeft*(leftCount-1) + leftDist)/leftCount;
            if (leftCount == usSensitivity) {
              Serial.println(leftCount);
              Serial.println("FOUND LEFT");
              return RIGHT;
            }
          }
        }
      } else {
        leftCount = 0;
      }
      Serial.print(oldRight);
      Serial.print(" ");
      Serial.print(rightDist);
      Serial.print(" ");
      Serial.print(rightCount);
      Serial.print("\n");

      Serial.print(oldLeft);
      Serial.print(" ");
      Serial.print(leftDist);
      Serial.print(" ");
      Serial.print(leftCount);
      Serial.print("\n");

      Serial.println("+++++++++++");
      return NOT_FOUND;
    }
};

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
  UsFilter* filter = new UsFilter(true, false);
  int found = NOT_FOUND;
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 20);
  HeadingReader hr;
  hr.heading = 0;
  float backDist = 0, frontDist = 0, rightDist = 0;

  // while(1) {
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

    // if (backDist < 40 && backDist != 0) {
    //   Serial.println("Forward");
    //   return FORWARD;
    // }
    // delay(60);
    hr.update(5);
  }
  turn(LEFT, 90 - (int) hr.heading);
  Serial.println("Not Found");
  return NOT_FOUND;
}

int moveOneThird() {
  DataManager dm;
  UsFilter* filter = new UsFilter(true, true);
  int found = NOT_FOUND;
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 1);
  HeadingReader hr;
  hr.heading = 0;

  float frontDist = 0, rightDist = 0, leftDist = 0, backDist = 0;
  bool passed_mid = false;
  
  // Split middle
  hr.update(10);
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
    hr.update(5);
  }

  turn(RIGHT, 90 + (int) hr.heading);
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 1);
  dm.updateBackUS();
  backDist = dm.getBackUS();
  hr.heading = 0;

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
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 1);
  dm.updateBackUS();
  delay(2);
  dm.updateLeftUS();
  delay(2);
  dm.updateFrontUS();
  passed_mid = false;

  hr.heading = 0;

  delete filter;
  filter = new UsFilter(false, true);
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

    found = filter->findBase(-1, leftDist);

    if (found != NOT_FOUND) {
      return found;
    }
    if (backDist < 40 && backDist != 0 && !passed_mid) {
      Serial.println("Forward");
      return FORWARD;
    }
    delay(50);
    hr.update(5);
  }
  return NOT_FOUND;
}

void searchForBase() {
  Serial.println("Searching");
  DataManager dm;
  DeviceState state;
  int found;
  found = moveSearchFront(90);
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
  while(1) {}
  return;
}
