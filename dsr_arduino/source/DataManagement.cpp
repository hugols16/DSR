#pragma once
#include <Wire.h>
#include <SPI.h>
#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include "SensorReading.cpp"
#include <math.h>

class DataManager {
private:
  static int usFront;
  static int usLeft;
  static int usRight;
  static int usBack;

  static LSM9DS1 imu_m;

  static float gX;
  static float gY;
  static float gZ;

  static float heading;

public:
  DataManager() {
    usFront = sensorReadingFront();
    usLeft = sensorReadingLeft();
    usRight = sensorReadingRight();
    usBack = sensorReadingBack();
  }

  int getFrontUS() { return usFront; }
  int getLeftUS() { return usLeft; }
  int getRightUS() { return usRight; }
  int getBackUS() { return usBack; }

  float getGyroX() { return gX; }
  float getGyroY() { return gY; }
  float getGyroZ() { return gZ; }

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

    updateGyro();
  }

  void updateGyro() {
    imu_m.readGyro();
    gY = imu_m.calcGyro(imu_m.gy) + 0.6683;
    gX = imu_m.calcGyro(imu_m.gx);
    gZ = imu_m.calcGyro(imu_m.gz) - 2.5637;
  }

  void updateLeftUS() {
    int leftReading = (int) sensorReadingLeft();
    usLeft = leftReading;
  }

  void updateRightUS() {
    int rightReading = (int) sensorReadingRight();
    usRight = rightReading;
  }

  void updateFrontUS() {
    int frontReading = (int) sensorReadingFront();
    usFront = frontReading;
  }

  void updateBackUS() {
    int backReading = (int) sensorReadingBack();
    if(backReading != 0) {
      usBack = backReading;
    }
  }

};

int DataManager::usFront = 0;
int DataManager::usLeft = 0;
int DataManager::usRight = 0;
int DataManager::usBack = 0;

float DataManager::gX = 0;
float DataManager::gY = 0;
float DataManager::gZ = 0;

LSM9DS1 DataManager::imu_m;
