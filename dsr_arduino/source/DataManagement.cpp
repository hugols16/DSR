#include "SensorReading.cpp"
// #include <queue>
struct DataValue {
  float value = 0;
  // int arr = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

class DataManager {
private:
  //data members go here
  // int sensorReading = 0;
  static DataValue usFront;
  static DataValue usLeft;
  static DataValue usRight;

public:
  //getters
  // int getSensorReading() { return sensorReading; }
  float getUsLeft() { return usLeft.value; }
  float getUsRight() { return usRight.value; }
  float getUsFront() { return usFront.value; }

  //updater
  void update() {
    //update new values
    // sensorReading = newSensorReading;

    usLeft.value = sensorReadingLeft();
    usRight.value = sensorReadingRight();
    usFront.value = sensorReadingFront();
  }
};

DataValue DataManager::usFront;
DataValue DataManager::usLeft;
DataValue DataManager::usRight;
