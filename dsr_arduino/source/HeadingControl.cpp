#pragma once
#include "Defines.h"
#include "DataManagement.cpp"

class HeadingReader {
private:
  static unsigned long t1;
  static unsigned long t2;
  static DataManager dm_m;

  float baseSpeed = 0.9;
  float variableSpeed = 0.1;

public:
  static float heading;

  HeadingReader () {
    DataManager dm_m;
  }

  void update(int iterations, int rightRatio=1, int leftRatio=1) {
    for (int i = 0; i < iterations; i++) {
      t2 = micros();

      if (t2 - t1 < 100000) {
        dm_m.updateGyro();
        heading += dm_m.getGyroZ() * (t2 - t1) * 0.000001;
        // Sigmoid function
        float multiplier = (2.0 / (1.0 + pow(2, (-0.75 * abs(heading)))) - 0.5) * (heading > 0 ? -1.0 : 1.0);
        move(-MAX_SPEED_RIGHT * rightRatio * (baseSpeed - variableSpeed * multiplier), -MAX_SPEED_LEFT * leftRatio * (baseSpeed + variableSpeed * multiplier), 1);
        // Serial.print("Heading diff: ");
        // Serial.print(heading);
        // Serial.print('\n');
      }

      t1 = t2;
      delay(10);
    }
  }
};

unsigned long HeadingReader::t1 = 0;
unsigned long HeadingReader::t2 = 0;
DataManager HeadingReader::dm_m;
float HeadingReader::heading = 0;
