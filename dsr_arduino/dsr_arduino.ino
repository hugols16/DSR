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

//void move(int speed_right, int speed_left, int ramp_step) {
//  int diff_right = speed_right - motorRight.getCurrentSpeed();
//  int diff_left = speed_left - motorLeft.getCurrentSpeed();
//
//  // If we aren't changing the values, simply return
//  if (diff_right == 0 && diff_left == 0) {
//    return;
//  }
//  for (int i=1; i <= ramp_step; i++) {
//      motorRight.drive(motorRight.getCurrentSpeed() + diff_right*i/ramp_step);
//      motorLeft.drive(motorLeft.getCurrentSpeed() + diff_left*i/ramp_step);
//      delay(10); //10 ms delay
//  }
//  motorRight.setCurrentSpeed(speed_right);
//  motorLeft.setCurrentSpeed(speed_left);
//}

//void ramp_searching() {
//  int sensor = 0;
//  float ultrasonic_left = dm->getLeftUS();
//  float ultrasonic_front = dm->getFrontUS();
//  float ultrasonic_back = dm->getBackUS();
//  float cur_mz = dm->getMagZ();
//  float new_mz = cur_mz;
//  switch(state->current) {
//  case RAMP_SEARCH:
//    move(MAX_SPEED_RIGHT*0.75, MAX_SPEED_LEFT*0.75, 10);
////    Serial.println("Searching for ramp");
//    Serial.println(ultrasonic_front);
//    while(!(ultrasonic_front < 70 && ultrasonic_front > 40)) {
//      Serial.println("Not yet 70");
//      dm->updateFrontUS();
//      ultrasonic_front = dm->getFrontUS();
//      Serial.println(ultrasonic_front);
//      delay(60);
//    }
//    while(ultrasonic_front > RAMP_DIST_X) {
//      Serial.println("Not yet 28");
//      dm->updateFrontUS();
//      ultrasonic_front = dm->getFrontUS();
//      Serial.println(ultrasonic_front);
//      delay(60);
//    }
//    state->transition();
//    break;
//  case RAMP_TURN:
//////      move(0,0,1);
////      move(-1*MAX_SPEED/2, MAX_SPEED/2, 4);
////      sensor = analogRead(IR_PIN);
////      // Get below 50
////      while(sensor > 170) {
////        sensor = analogRead(IR_PIN);
////        Serial.println(sensor);
//////        delay(50);
////      }
////
////      while(sensor < 220) {
////        sensor = analogRead(IR_PIN);
////        Serial.println(sensor);
//////        delay(50);
////      }
////      move(MAX_SPEED/2, MAX_SPEED/2, 1);
//      turn(LEFT, 90, dm);
//      state->transition();
//    break;
//
//  case RAMP_AHEAD:
//    Serial.println("RAMP_AHEAD");
//    move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 20);
////    level_mz = dm->getMagZ();
////    Serial.println(level_mz);
////    new_mz = cur_mz;
////    Serial.println(new_mz);
////    while(abs(level_mz - new_mz) < ramp_change) {
////      dm->updateMag();
////      new_mz = dm->getMagZ();
////    }
//    waitFor(UP, 25, dm);
//    delay(500);
//    state->transition();
//    break;
//  }
//}

void ramp_moving() {
  float ramp_up_mz = dm->getMagZ();
  float new_mz = ramp_up_mz;
  float ultrasonic_front = dm->getFrontUS();
  delay(100);
  float ultrasonic_left =  dm->getLeftUS();
  delay(100);
  float heading_diff = 0;
  float min = 1000; // stupid high
  int count = 0, sensitivity = 4;
  switch(state->current) {
  case RAMP_UP:
    Serial.println("RAMP_UP");
    move(MAX_SPEED_RIGHT, MAX_SPEED_LEFT, 20);
    ramp_up_mz = dm->getMagZ();
    new_mz = ramp_up_mz;
    Serial.println(new_mz);
//    delay(7000); //Ensure that the car is on the ramp
//    while(abs(new_mz -level_mz) > 0.02) {
//      dm->updateMag();
//      new_mz = dm->getMagZ();
//      Serial.println(new_mz);
//      delay(100);
//    }
    waitFor(DOWN, 25, dm);
    delay(10);
    state->transition();
//    state->transition();
    break;
  case RAMP_LEVEL:
//    move(MAX_SPEED_RIGHT*0.75, MAX_SPEED_LEFT*0.75, 20);
    move(MAX_SPEED_RIGHT/2,MAX_SPEED_LEFT/2,10);
    waitFor(DOWN, 25, dm);
    state->transition();
  case RAMP_DOWN:
    // Turn at the bottom
    Serial.println("RAMP_DOWN");
    move(MAX_SPEED_RIGHT*0.1,MAX_SPEED_LEFT*0.1,1);
//      move(0,0,1);
      heading_diff = getHeadingDiff(dm);
      turn(RIGHT, 90 + heading_diff, dm);
//    move(-MAX_SPEED_RIGHT/2, MAX_SPEED_LEFT/2, 1); // Brake on the way down
//    waitFor(UP, 20, dm);
//    while(!(ultrasonic_left > 5 && ultrasonic_left < 40) || !(ultrasonic_front > min && count > sensitivity)) {
//        turn(RIGHT, 5, dm);
//        dm->updateFrontUS();
//        delay(60);
//        dm->updateLeftUS();
//        delay(60);
//        ultrasonic_left = dm->getLeftUS();
//        if (ultrasonic_front > min) {
//          count++;
//        } else {
//          min = ultrasonic_front;
//          count = 0;
//        }
//        ultrasonic_front = dm->getFrontUS();
//        Serial.println(ultrasonic_front);
//        Serial.println(ultrasonic_left);
//    }
    move(0,0,1);
    state->transition();
    break;
  }
}

void base_searching() {
  Serial.println("base search");
//  move(MAX_SPEED_RIGHT/3, MAX_SPEED_LEFT/3, 3);
//  delay(500);
//  float USFront = dm->getFrontUS();
//  while(USFront > 20) {
//    dm->updateFrontUS();
//  }
  move(0,0,1);
  Serial.println("Done");
//  state->transition();
}
void loop() {
  // Update Sensor Values
  dm->update();
//  Serial.println(dm->getGyroY());
//  Serial.println("Hello");
//  float sensor = analogRead(IR_PIN);
//  Serial.println(sensor);
//  float dist = dm->getFrontUS();
//  Serial.println(dist);
//  Serial.println("Not it");
//  waitFor(DOWN, 10, dm);
//  Serial.println("Yep");
//  getHeadingDiff(dm);
  switch(state->current) {
    case READY:
//      state->transition();
//      state->transition();
//      state->transition();
//      state->transition();
//      state->transition();
//      state->transition();
      break;
    case RAMP_SEARCH:
    case RAMP_TURN:
    case RAMP_AHEAD:
      ramp_searching(dm, state);
      break;
    case RAMP_UP:
    case RAMP_LEVEL:
    case RAMP_DOWN:
      ramp_moving();
      break;
    case SEARCHING:
    base_searching();
      break;
  }
//  imu.readMag(); // Update the magnetometer data
//  Serial.println(imu.calcMag(imu.mz)); // print z-axis data
  delay(100);
}
