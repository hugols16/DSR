#pragma once
#include "DataManagement.cpp"
#include "Defines.h"

void waitFor(int dir, int deg, DataManager * dm) {
  float current_deg = 0;
  int t1 = micros(), t2;

  if(dir == UP) {
    while(current_deg < deg) {
      t2 = t1;
      t1 = micros();
      dm->updateGyro();
      current_deg += dm->getGyroY() * (t1 - t2) * 0.000001;
    }
  } else if (dir == DOWN) {
    while (current_deg > -1.0*deg) {
      t2 = t1;
      t1 = micros();
      dm->updateGyro();
      current_deg += dm->getGyroY() * (t1 - t2) * 0.000001;
    }
  } else {
    Serial.println("Inavlid waitFor Heading direction");
    while(1) {}
  }
}
