#pragma once
#include <Wire.h>
#include <SPI.h>
#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include "SensorReading.cpp"
#include <math.h>

const int sensitivity = 3;

class DataManager {
private:
  static int usFront;
  static int frontDiff;
  static int usLeft;
  static int leftDiff;
  static int usRight;
  static int rightDiff;
  static int usBack;
  static int backDiff;

  LSM9DS1 imu_m;

  static float aX;
  static float aY;
  static float aZ;

  static float gX;
  static float gY;
  static float gZ;

  static float mX;
  static float mY;
  static float mZ;
  static float heading;

  static int ir;
  int ir_val[12] = { 517, 409, 313, 254, 214, 186, 167, 148, 135, 123, 113, 104 };

public:
  DataManager() {
    // usFront = sensorReadingFront();
    // usLeft = sensorReadingLeft();
    // usRight = sensorReadingRight();
    // usBack = sensorReadingBack();
  }

  int getFrontUS() { return usFront; }
  int getLeftUS() { return usLeft; }
  int getRightUS() { return usRight; }
  int getBackUS() { return usBack; }

  float getAccX() { return aX; }
  float getAccY() { return aY; }
  float getAccZ() { return aZ; }

  float getGyroX() { return gX; }
  float getGyroY() { return gY; }
  float getGyroZ() { return gZ; }

  float getHeading() { return heading; }
  float getMagX() { return mX; }
  float getMagY() { return mY; }
  float getMagZ() { return mZ; }

  float getIR() { return ir; }

  void setDataManagerIMU(LSM9DS1 new_imu) {
    imu_m = new_imu;
  }

  //updater
  void update() {
    delay(5);
    updateFrontUS();
    delay(5);
    updateLeftUS();
    delay(5);
    updateRightUS();
    delay(5);
    updateBackUS();

    updateAccel();
    updateGyro();
    updateMag();
    updateIR();
  }

  void updateIR() {
    int sensorValue = analogRead(12);
    if (sensorValue > 517) {
      ir = 0;
    }
    if (sensorValue < 104) {
      ir = 0;
    }

    int i = 0;
    while(ir_val[i] >= sensorValue) {
      i++;
    }

    ir = ir * 0.3 + 0.7 * ((int) (10 + 10 * i + 10 * (ir_val[i-1] - sensorValue) / (ir_val[i-1] - ir_val[i])));
  }

  void updateMag() {
    imu_m.readMag();
    // Hardcoded values are experimental center of ellipse values.
    mX = imu_m.calcMag(imu_m.mx) * 100.0 + 12.3672 + 10;
    mY = imu_m.calcMag(imu_m.my) * 100.0  - 2.7035 + 2.5;
    mZ = imu_m.calcMag(imu_m.mz);
    // heading = atan(mX / mY) * 180 / PI;
    if(mY < 0.0) {
      heading = atan(mX / mY) * 180 / PI + 180.0;
    } else {
      heading = atan(mX / mY) * 180 / PI;
    }
  }

  void updateGyro() {
    imu_m.readGyro();
    gY = imu_m.calcGyro(imu_m.gy) + 0.6683;
    gX = imu_m.calcGyro(imu_m.gx);
    gZ = imu_m.calcGyro(imu_m.gz) - 2.5637;
  }

  void updateAccel() {
    imu_m.readAccel();
    aY = imu_m.calcAccel(imu_m.ay);
    aX = imu_m.calcAccel(imu_m.ax);
    aZ = imu_m.calcAccel(imu_m.az);
  }

  void updateLeftUS() {
    int leftReading = (int) sensorReadingLeft();
    if (leftReading == 0) return;
    if (leftReading != usLeft) {
      leftDiff++;
    } else { leftDiff = 0; }
    if ( leftDiff >= sensitivity ) {
      usLeft = leftReading;
      leftDiff = 0;
    }
  }

  void updateRightUS() {
    int rightReading = (int) sensorReadingRight();
    if (rightReading == 0) return;
    if (rightReading != usRight) {
      rightDiff++;
    } else {
      rightDiff = 0;
    }
    if ( rightDiff >= sensitivity ) {
      usRight = rightReading;
      rightDiff = 0;
    }
  }

  void updateFrontUS() {
    int frontReading = (int) sensorReadingFront();
    if (frontReading == 0) return;
    if ( frontReading != usFront) {
      frontDiff++;
    } else {
      frontDiff = 0;
    }
    if( frontDiff >= sensitivity ) {
      usFront = frontReading;
      frontDiff = 0;
    }
  }

  void updateBackUS() {
    int backReading = (int) sensorReadingBack();
    if (backReading == 0) return;
    if ( backReading != usBack) {
      backDiff++;
    } else {
      backDiff = 0;
    }
    if( backDiff >= sensitivity ) {
      usBack = backReading;
      backDiff = 0;
    }
  }

};

int DataManager::usFront = 0;
int DataManager::frontDiff = 0;
int DataManager::usLeft = 0;
int DataManager::leftDiff = 0;
int DataManager::usRight = 0;
int DataManager::rightDiff = 0;
int DataManager::usBack = 0;
int DataManager::backDiff = 0;

float DataManager::aX = 0;
float DataManager::aY = 0;
float DataManager::aZ = 0;

float DataManager::gX = 0;
float DataManager::gY = 0;
float DataManager::gZ = 0;

float DataManager::mX = 0;
float DataManager::mY = 0;
float DataManager::mZ = 0;
float DataManager::heading = 0;

int DataManager::ir = 0;
