#pragma once
#include "Defines.h"

class HeadingReader {
private:
  static unsigned long t1;
  static unsigned long t2;
  static DataManager dm_m;
public:
  static float heading;

  HeadingReader () {
    DataManager dm_m;
  }

  void update() {
    t2 = micros();

    if (t2 - t1 < 100000) {
      dm_m.updateGyro();
      heading += dm_m.getGyroZ() * (t2 - t1) * 0.000001;
    }

    t1 = t2;
  }
};

unsigned long HeadingReader::t1 = 0;
unsigned long HeadingReader::t2 = 0;
DataManager HeadingReader::dm_m;
float HeadingReader::heading = 0;
