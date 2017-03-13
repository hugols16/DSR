#include <Wire.h>
#include <SPI.h>

#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include <SparkFunLSM9DS1.h>

#include <SparkFun_TB6612.h>

#include "source/Defines.h"
#include "source/DeviceState.cpp"
#include "source/DataManagement.cpp"
#include "source/MotorControl.cpp"
#include "source/RampSearching.cpp"


// Create IMU object
LSM9DS1 imu;

// Create Global State Machine
DeviceState* state;

// Create Global Data Manager
DataManager* dm;

//Might want to put the following in the Defines
// Anything over 400 cm (23200 us pulse) is "out of range"
//const unsigned int MAX_DIST = 400.0;

float level_mz = 0;
float const ramp_change = 0.07;

//float readSensor() {
//  unsigned long t1;
//  unsigned long t2;
//  unsigned long pulse_width;
//  float cm;
//  float inches;
//
//  // Hold the trigger pin high for at least 10 us
//  digitalWrite(TRIG_PIN, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(TRIG_PIN, LOW);
//  Serial.println("Yo");
//  // Wait for pulse on echo pin
//  while ( digitalRead(ECHO_PIN) == 0 );
//  // Measure how long the echo pin was held high (pulse width)
//  // Note: the micros() counter will overflow after ~70 min
//  t1 = micros();
//  while ( digitalRead(ECHO_PIN) == 1);
//  t2 = micros();
//  pulse_width = t2 - t1;
//
//  // Calculate distance in centimeters and inches. The constants
//  // are found in the datasheet, and calculated from the assumed speed
//  //of sound in air at sea level (~340 m/s).
//  return pulse_width / 58.0;
//}

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
  
  delay(20);
}
