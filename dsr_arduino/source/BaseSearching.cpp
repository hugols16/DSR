#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"
#include "HeadingControl.cpp"

float searchSpeed = 0.7;

// class UsFilter {
//   private:
//     // bool usRight, usLeft;
//     int rightCount, leftCount;
//     int oldRight, oldLeft;
//     int usNoise, usSensitivity, usMaxDist;

//   public:
//     UsFilter(int sensitivity=5, int noise=2, int max_dist=50) {
//       // US flags
//       // usRight = right;
//       // usLeft = left;
//       // US counts
//       rightCount = 0;
//       leftCount = 0;
//       // Keeping track of old values
//       oldRight = 0;
//       oldLeft = 0;
//       // Allowable noise
//       usNoise = noise;
//       // Algorithm sensitivity
//       usSensitivity = sensitivity;

//       // Max Allowable Distance
//       usMaxDist = max_dist;
//     }

//     int findBase(int rightDist, int leftDist) {
//       //Right US
//       if (rightDist > 0 && rightDist < usMaxDist) {
//         // Start new cycle
//         if (rightCount == 0) {
//           oldRight = rightDist;
//           rightCount++;
//         } else {
//           if (abs(rightDist - oldRight) > usNoise) {
//             rightCount = 1;
//             oldRight = rightDist;
//           } else {
//             rightCount++;
//             // Get average of the values
//             oldRight = (oldRight*(rightCount-1) + rightDist)/rightCount;
//             if (rightCount == usSensitivity) {
//               Serial.println(rightCount);
//               Serial.println("FOUND RIGHT");
//               return LEFT;
//             }
//           }
//         }
//       } else {
//         rightCount = 0;
//       }
//       //Left US
//       if (leftDist > 0 && leftDist < usMaxDist) {
//         // Start new cycle
//         if (leftCount == 0) {
//           oldLeft = leftDist;
//           leftCount++;
//         } else {
//           if (abs(leftDist - oldLeft) > usNoise) {
//             leftCount = 1;
//             oldLeft = leftDist;
//           } else {
//             leftCount++;
//             // Get average of the values
//             oldLeft = (oldLeft*(leftCount-1) + leftDist)/leftCount;
//             if (leftCount == usSensitivity) {
//               Serial.println(leftCount);
//               Serial.println("FOUND LEFT");
//               return RIGHT;
//             }
//           }
//         }
//       } else {
//         leftCount = 0;
//       }
//       // Serial.print(oldRight);
//       // Serial.print(" ");
//       // Serial.print(rightDist);
//       // Serial.print(" ");
//       // Serial.print(rightCount);
//       // Serial.print("\n");

//       // Serial.print(oldLeft);
//       // Serial.print(" ");
//       // Serial.print(leftDist);
//       // Serial.print(" ");
//       // Serial.print(leftCount);
//       // Serial.print("\n");

//       // Serial.println("+++++++++++");
//       return NOT_FOUND;
//     }
// };

int checkSides(bool right=false, bool left=false, bool front=false, float iterations=3, int range=60, int frontRange=40, float sensitivity=1.05, int backTargetDist=999) {
  DataManager dm;
  HeadingReader hr;
  int us_right = 0;
  int us_left = 0;
  int us_back = 0;
  int us_front = 0;
  float mean_right = 0;
  float mean_left = 0;
  int countRight = 0, countLeft = 0;
  bool zero = false;
  while((right && countRight < iterations) || (left && countLeft < iterations)) {
  // for(int i=0; i<iterations; i++) {

    dm.updateBackUS();
    us_back = dm.getBackUS();
    if (us_back > backTargetDist) return BACK;

    if (front) {
      dm.updateFrontUS();
      us_front = dm.getFrontUS();
      if (us_front && us_front < frontRange) return FRONT;
    }
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
    hr.updateForward(3, searchSpeed, searchSpeed);
  }
  if (right && mean_right < range) return RIGHT;
  if (left && mean_left < range) return LEFT;

  return NOT_FOUND;
}

int moveSearchFront(int targetDist) {
  DataManager dm;
  // UsFilter* filter = new UsFilter();
  int found = NOT_FOUND;
  move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 20);
  HeadingReader hr;
  hr.heading = 0;
  float backDist = 0, frontDist = 0, rightDist = 0;

  // So we don't see the ramp
  while(backDist < 30) {
    dm.updateBackUS();
    backDist = dm.getBackUS();
    hr.updateForward(4, searchSpeed, searchSpeed);
  }
  while(1) {

    // Use base algorithm
    found = checkSides(false, true, true, 3.0, 60, 40, 1.1, targetDist);
    if (found == BACK) {
      break;
    }

    if (found != NOT_FOUND) return found;

  }
  turn(LEFT, 90 - (int) hr.heading);
  return NOT_FOUND;
}

int moveOneThird() {
  DataManager dm;
  int found = NOT_FOUND;
  move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 1);
  HeadingReader hr;
  hr.heading = 0;

  float frontDist = 0, rightDist = 0, leftDist = 0, backDist = 0;
  bool passed_mid = false;

  // Split middle
  hr.updateForward(50, searchSpeed, searchSpeed);
  while(1) {
    // Use base algorithm
    found = checkSides(true, true, true, 3.0, 60, 12, 1.05, 999);

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
  found = moveSearchFront(115);
  if (found != NOT_FOUND) {
    state.transition();
    hr.updateFoundBase(found);
  }
  found = moveOneThird();
  if (found != NOT_FOUND) {
    hr.updateFoundBase(found);
  }
  state.transition();
  state.transition();
  move(0,0,1);
  while(1) {}
  return;
}
