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
  motorRight.drive(motorRight.getCurrentSpeed() + diffRight/ramp_step);
  motorLeft.drive(motorLeft.getCurrentSpeed() + diffLeft/ramp_step);

  for (int i=2; i <= ramp_step; i++) {
      delay(10); //10 ms delay
      motorRight.drive(motorRight.getCurrentSpeed() + diffRight*i/ramp_step);
      motorLeft.drive(motorLeft.getCurrentSpeed() + diffLeft*i/ramp_step);
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

  float speedRatio = 0.4;
  move(dir ? MAX_SPEED_RIGHT*speedRatio : -MAX_SPEED_RIGHT*speedRatio,  dir ? -MAX_SPEED_LEFT*speedRatio : MAX_SPEED_LEFT*speedRatio, 5);

  int count  = 0;
  float prevDeg = -999;
  while(abs(currentDeg - deg) > 4.0) {
    t2 = t1;
    t1 = micros();
    dm.updateGyro();
    currentDeg += (dm.getGyroZ() * (t1 - t2) * 0.000001) * (dir ? 1.0 : -1.0);
    currentDeg = currentDeg - ((int)(currentDeg / 360.0)) * 360.0;
    delay(5);

    count++;
    if(count % 100 == 0) {
      if(abs(prevDeg - currentDeg) < 5) {
        if(count % 200 == 0) {
          move(-MAX_SPEED_RIGHT*speedRatio, -MAX_SPEED_LEFT*speedRatio, 1);
        } else {
          move(MAX_SPEED_RIGHT*speedRatio, MAX_SPEED_LEFT*speedRatio, 1);
        }
        delay(200);
        move(dir ? MAX_SPEED_RIGHT*speedRatio : -MAX_SPEED_RIGHT*speedRatio,  dir ? -MAX_SPEED_LEFT*speedRatio : MAX_SPEED_LEFT*speedRatio, 5);
      }
      t1 = micros();
      prevDeg = currentDeg;
    }
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
    delayMicroseconds(800);
    currentDeg += dm.getGyroZ() * (t1 - t2) * 0.000001;
  }

  return currentDeg;
}
