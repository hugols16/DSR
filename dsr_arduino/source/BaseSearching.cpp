#pragma once
#include "MotorControl.cpp"
#include "DataManagement.cpp"

float ratio = 1;
void searchForBase(DataManager * dm) {
  // Update Sensor Values
  turn(LEFT, 90, dm);
  int front_dist = 1000;
  move(MAX_SPEED_RIGHT*ratio, MAX_SPEED_LEFT*ratio, 3);

  int count = 0;
  while(count < 20) {
    dm->updateFrontUS();
    front_dist = dm->getFrontUS();

    if (front_dist < 90) {
      count++;
    } else {
      count = 0;
    }
    Serial.print("front_dist: ");
    Serial.print(front_dist);
    Serial.print("\n");
    delay(20);
  }

  turn(LEFT, 90, dm);

  for(int i = 0; i < 10; i++) {
    dm->updateRightUS();
    int tmp = dm->getRightUS();
    delay(10);
  }

  dm->updateRightUS();
  int start_dist = dm->getRightUS();
  int right_dist = start_dist;

  count = 0;
  move(MAX_SPEED_RIGHT*ratio, MAX_SPEED_LEFT*ratio, 3);
  while(count < 20) {
    dm->updateRightUS();
    right_dist = dm->getRightUS();
    if (abs(start_dist - right_dist) > 30) {
      count ++;
    } else {
      count = 0;
    }
    Serial.print("right_dist: ");
    Serial.print(right_dist);
    Serial.print("\n");
    delay(20);
  }

  turn(LEFT, 90, dm);
  move(-MAX_SPEED_RIGHT, -MAX_SPEED_LEFT, 10);

  delay(2000);
  move(0, 0, 1);
}
