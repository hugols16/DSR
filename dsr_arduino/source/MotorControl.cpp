#pragma once
#include "Defines.h"
// #include <math.h>

class DSRMotor: public Motor {
  private:
    int curSpeed;
  public:
    DSRMotor(int In1pin, int In2pin, int PWMpin, int offset, int STBYpin):
    Motor(In1pin, In2pin, PWMpin, offset, STBYpin) {
      curSpeed = 0;
    }
    int getCurrentSpeed() {return curSpeed;}
    void setCurrentSpeed(int newSpeed) {curSpeed = newSpeed;}
};

// Create motor objects
DSRMotor motorLeft = DSRMotor(AIN1, AIN2, PWMA, offsetA, STBY);
DSRMotor motorRight = DSRMotor(BIN1, BIN2, PWMB, offsetB, STBY);

void move(int speedRight, int speedLeft, int ramp_step) {
  int diffRight = speedRight - motorRight.getCurrentSpeed();
  int diffLeft = speedLeft - motorLeft.getCurrentSpeed();

  // If we aren't changing the values, simply return
  if (diffRight == 0 && diffLeft == 0) {
    return;
  }
  for (int i=1; i <= ramp_step; i++) {
      motorRight.drive(motorRight.getCurrentSpeed() + diffRight*i/ramp_step);
      motorLeft.drive(motorLeft.getCurrentSpeed() + diffLeft*i/ramp_step);
      delay(10); //10 ms delay
  }
  motorRight.setCurrentSpeed(diffRight);
  motorLeft.setCurrentSpeed(diffLeft);
}

// dir 0 -> right turn ,  dir 1 -> left turn
void turn(bool dir, float deg) {
  DataManager dm;
  float currentDeg = 0;
  unsigned long t1 = micros(), t2;

  move(0,0,1);
  delay(200);

  float speedRatio = 0.3;
  move(dir ? MAX_SPEED_RIGHT*speedRatio : -MAX_SPEED_RIGHT*speedRatio,  dir ? -MAX_SPEED_LEFT*speedRatio : MAX_SPEED_LEFT*speedRatio, 5);

  while(abs(currentDeg - deg) > 4.0) {
    t2 = t1;
    t1 = micros();
    dm.updateGyro();
    currentDeg += (dm.getGyroZ() * (t1 - t2) * 0.000001) * (dir ? 1.0 : -1.0);
    delay(5);
  }

  move(0, 0, 1);
}

float getHeadingDiff() {
  DataManager dm;
  float currentDeg = 0;
  unsigned long t1 = micros(), t2;
  delay(2);
  unsigned long startTime = t1;

  while(t1 - startTime < 2500000) {
    t2 = t1;
    t1 = micros();
    dm.updateGyro();
    delay(1);
    currentDeg += dm.getGyroZ() * (t1 - t2) * 0.000001;
  }

  return currentDeg;
}

void moveDist(float targetDist) {
  DataManager dm;

  float dist = 0, vel = 0, acc;
  unsigned long t1 = micros(), t2;

  move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 3);

  while(dist < targetDist) {
    t2 = t1;
    t1 = micros();
    dm.updateAccel();
    vel += dm.getAccX() * (t1 - t2) * 0.000001;
    dist += vel * (t1 - t2) * 0.000001;
    Serial.print(dm.getAccX());
    // Serial.print(' ');
    // Serial.print(vel);
    Serial.print('\n');
    delay(5);
  }

  move(0, 0, 1);
}
