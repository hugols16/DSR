#include <Wire.h>
#include <SPI.h>

#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include <SparkFunLSM9DS1.h>

#include  <SparkFun_TB6612.h>

#include "source/Defines.h"
#include "source/DeviceState.cpp"
#include "source/DataManagement.cpp"
#include "source/MotorControl.cpp"
#include "source/RampSearching.cpp"
#include "source/PitchControl.cpp"
#include "source/RampMoving.cpp"

// Create IMU object
LSM9DS1 imu;

// Create Global State Machine
DeviceState* state;

// Create Global Data Manager
DataManager* dm;

void setup() {

  // Setting up the imu
  imu.settings.device.commInterface = IMU_MODE_I2C; // Set mode to I2C
  imu.settings.device.mAddress = LSM9DS1_M; // Set mag address to 0x1E
  imu.settings.device.agAddress = LSM9DS1_AG; // Set ag address to 0x6B
  imu.settings.accel.scale = 4; // Set accel range to +/-4g
  imu.settings.gyro.scale = 500; // Set gyro range to +/-720dps
  imu.settings.mag.scale = 2; // Set mag range to +/-2Gs

  if (!imu.begin()) {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Looping to infinity.");
    while (1);
  }

  // Set up the Ultrasonic Sensors
  pinMode(TRIG_PIN_FRONT, OUTPUT);
  digitalWrite(TRIG_PIN_FRONT, LOW);
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  digitalWrite(TRIG_PIN_RIGHT, LOW);
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  digitalWrite(TRIG_PIN_LEFT, LOW);
  pinMode(TRIG_PIN_BACK, OUTPUT);
  digitalWrite(TRIG_PIN_BACK, LOW);

  delay(100);
  // Create Data Manager
  dm = new DataManager();
  dm->setDataManagerIMU(imu);

  // Create State Machine
  state = new DeviceState();

  Serial.begin(9600);

  // Move to READY
  state->transition();
}

void loop() {
  // Update Sensor Values
  dm->update();
  switch(state->current) {
    case READY:
      state->transition();
      break;
    case RAMP_SEARCH:
    case RAMP_TURN:
    case RAMP_AHEAD:
      ramp_searching(dm, state);
      break;
    case RAMP_UP:
    case RAMP_LEVEL:
    case RAMP_DOWN:
      ramp_moving(dm, state);
      break;
    case SEARCHING:
      break;
  }
  delay(100);
}
