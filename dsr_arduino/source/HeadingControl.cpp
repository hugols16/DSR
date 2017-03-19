#pragma once
#include "Defines.h"

class HeadingReader {
private:
  static unsigned long t1;
  static unsigned long t2;
  static DataManager dm_m;
public:
  static float heading;
  static float target;

  HeadingReader () {
    DataManager dm_m;
  }

  void update() {
    t2 = micros();

    if (t2 - t1 < 100000) {
      dm_m.updateGyro();
      heading += dm_m.getGyroZ() * (t2 - t1) * 0.000001;
      // Sigmoid function
      float multiplier = 2.0 / (1.0 + pow(2, (-0.75 * abs(heading)))) - 0.5 * heading > 0 ? 1.0 : -1.0;
      move(-MAX_SPEED_RIGHT * (0.8 - 0.2 * multiplier), -MAX_SPEED_LEFT * (0.8 + 0.2 * multiplier), 1);
    }

    t1 = t2;
  }
};

unsigned long HeadingReader::t1 = 0;
unsigned long HeadingReader::t2 = 0;
DataManager HeadingReader::dm_m;
float HeadingReader::heading = 0;
float HeadingReader::target = 0;
