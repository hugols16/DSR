#pragma once
#include <Wire.h>
#include <SPI.h>
#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include "SensorReading.cpp"
#include <math.h>

const int sensitivity = 5;

class DataManager {
private:
  static int usFront;
  static int frontDiff;
  static int usLeft;
  static int leftDiff;
  static int usRight;
  static int rightDiff;

  LSM9DS1 imu_m;

  static float aX;
  static float aY;
  static float aZ;

  static float mX;
  static float mY;
  static float heading;


public:
  int getUsFront() { return usFront; }
  int getUsLeft() { return usLeft; }
  int getUsRight() { return usRight; }

  float getAccX() { return aX; }
  float getAccY() { return aY; }
  float getAccZ() { return aZ; }

  float getHeading() { return heading; }
  float getMagX() { return mX; }
  float getMagY() { return mY; }

  void setDataManagerIMU(LSM9DS1 new_imu) {
    imu_m = new_imu;
  }

  //updater
  void update() {
    updateFrontUS();
    updateLeftUS();
    updateRightUS();

    updateAccel();
    updateMag();
  }

  void updateMag() {
    imu_m.readMag();
    // Hardcoded values are experimental center of ellipse values.
    mX = imu_m.calcMag(imu_m.mx) * 100.0 + 12.3672 + 10;
    mY = imu_m.calcMag(imu_m.my) * 100.0  - 2.7035 + 2.5;
    // heading = atan(mX / mY) * 180 / PI;
    if(mY < 0.0) {
      heading = atan(mX / mY) * 180 / PI + 180.0;
    } else {
      heading = atan(mX / mY) * 180 / PI;
    }


  }

  void updateAccel() {
    imu_m.readAccel();
    aY = imu_m.calcAccel(imu_m.ay);
    aX = imu_m.calcAccel(imu_m.ax);
    aZ = imu_m.calcAccel(imu_m.az);
  }

  void updateLeftUS() {
    int leftReading = (int) sensorReadingLeft();
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
    if (rightReading != usRight) {
      rightDiff++;
    } else { rightDiff = 0; }
    if ( rightDiff >= sensitivity ) {
      usRight = rightReading;
      rightDiff = 0;
    }
  }

  void updateFrontUS() {
    int frontReading = (int) sensorReadingFront();
    if ( frontReading != usFront) {
      frontDiff++;
    } else { frontDiff = 0; }
    if( frontDiff >= sensitivity ) {
      usFront = frontReading;
      frontDiff = 0;
    }
  }

};

int DataManager::usFront = 0;
int DataManager::frontDiff = 0;
int DataManager::usLeft = 0;
int DataManager::leftDiff = 0;
int DataManager::usRight = 0;
int DataManager::rightDiff = 0;

float DataManager::aX = 0;
float DataManager::aY = 0;
float DataManager::aZ = 0;

float DataManager::mX = 0;
float DataManager::mY = 0;
float DataManager::heading = 0;
