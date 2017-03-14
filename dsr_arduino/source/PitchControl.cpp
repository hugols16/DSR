#pragma once
#include "DataManagement.cpp"
#include "Defines.cpp"

void waitFor(int dir, int deg, DataManager * dm) {
  float current_deg = 0;

  if(dir == UP) {
    while(current_deg < deg) {
      dm->updateGyro();
      current_deg += dm->getGyroY();
    }
  } else if (dir == DOWN) {
    while (current_deg > deg) {
      dm->updateGyro();
      current_deg += dm->getGyroY();
    }
  } else {
    Serial.println("Inavlid waitFor Heading direction");
    while(1) {}
  }
}
