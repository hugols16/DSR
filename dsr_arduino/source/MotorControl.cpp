#pragma once
#include "Defines.h"
// #include <math.h>

class DSRMotor: public Motor {
  private:
    int cur_speed;
  public:
    DSRMotor(int In1pin, int In2pin, int PWMpin, int offset, int STBYpin):
    Motor(In1pin, In2pin, PWMpin, offset, STBYpin) {
      cur_speed = 0;
    }
    int getCurrentSpeed() {return cur_speed;}
    void setCurrentSpeed(int new_speed) {cur_speed = new_speed;}
};

// Create motor objects
DSRMotor motorLeft = DSRMotor(AIN1, AIN2, PWMA, offsetA, STBY);
DSRMotor motorRight = DSRMotor(BIN1, BIN2, PWMB, offsetB, STBY);

void move(int speed_right, int speed_left, int ramp_step) {
  int diff_right = speed_right - motorRight.getCurrentSpeed();
  int diff_left = speed_left - motorLeft.getCurrentSpeed();

  // If we aren't changing the values, simply return
  if (diff_right == 0 && diff_left == 0) {
    return;
  }
  for (int i=1; i <= ramp_step; i++) {
      motorRight.drive(motorRight.getCurrentSpeed() + diff_right*i/ramp_step);
      motorLeft.drive(motorLeft.getCurrentSpeed() + diff_left*i/ramp_step);
      delay(10); //10 ms delay
  }
  motorRight.setCurrentSpeed(speed_right);
  motorLeft.setCurrentSpeed(speed_left);
}

// dir 0 -> right turn ,  dir 1 -> left turn
void turn(bool dir, float deg, DataManager * dm) {
  float current_deg = 0;
  int t1 = micros(), t2;
//  float v1 = 0, v2 = 0, v3 = 0;
  float speed_ratio = 0.5;
  move(dir ? MAX_SPEED_RIGHT*speed_ratio : -MAX_SPEED_RIGHT*speed_ratio,  dir ? -MAX_SPEED_LEFT*speed_ratio : MAX_SPEED_LEFT*speed_ratio, 5);

  while(abs(current_deg - deg) > 4.0) {
    t2 = t1;
    t1 = micros();
    dm->updateGyro();
//    v3 = v2;
//    v2 = v1;
//    v1 = dm->getGyroZ();
//    float vel =  v1 * 0.4 + 0.3 * v2 + 0.3 * v3;
    current_deg += (dm->getGyroZ() * (t1 - t2) * 0.000001) * (dir ? 1.0 : -1.0);
    delay(5);
  }

  move(0, 0, 1);
}
