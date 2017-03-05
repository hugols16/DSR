#include "SensorReading.cpp"

class DataManager {
private:
  static int usFront;
  static int frontDiff;
  static int usLeft;
  static int leftDiff;
  static int usRight;
  static int rightDiff;

public:
  int getUsFront() { return usFront; }
  int getUsLeft() { return usLeft; }
  int getUsRight() { return usRight; }

  //updater
  void update() {
    int frontReading = (int) sensorReadingFront();
    int leftReading = (int) sensorReadingLeft();
    int rightReading = (int) sensorReadingRight();

    if ( frontReading != usFront) {
      frontDiff++;
    } else { frontDiff = 0; }
    if( frontDiff >= 10 ) {
      usFront = frontReading;
      frontDiff = 0;
    }

    if (leftReading != usLeft) {
      leftDiff++;
    } else { leftDiff = 0; }
    if ( leftDiff >= 10 ) {
      usLeft = leftReading;
      leftDiff = 0;
    }

    if (rightReading != usRight) {
      rightDiff++;
    } else { rightDiff = 0; }
    if ( rightDiff >= 10 ) {
      usRight = rightReading;
      rightDiff = 0;
    }
  }
};

int DataManager::usFront = 0;
int DataManager::frontDiff = 0;
int DataManager::usLeft = 0;
int DataManager::leftDiff = 0;
int DataManager::usRight = 0;
int DataManager::rightDiff = 0;
