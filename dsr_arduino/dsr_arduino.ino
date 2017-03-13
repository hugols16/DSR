#include <Wire.h>
#include <SPI.h>

#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include <SparkFunLSM9DS1.h>

#include <SparkFun_TB6612.h>

#include "source/Defines.h"
#include "source/DeviceState.cpp"
#include "source/DataManagement.cpp"

class DSRMotor: public Motor {
  private:
    int cur_speed;
  public:
    DSRMotor(int In1pin, int In2pin, int PWMpin, int offset, int STBYpin):
    Motor(In1pin, In2pin, PWMpin, offset, STBYpin) {
      cur_speed = 0;  
    }
    int getCurrentSpeed() {return cur_speed;}
    void setCurrentSpeed(int new_speed) {cur_speed = new_speed;}      
};

// Create motor objects
DSRMotor motorLeft = DSRMotor(AIN1, AIN2, PWMA, offsetA, STBY);
DSRMotor motorRight = DSRMotor(BIN1, BIN2, PWMB, offsetB, STBY);

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
  imu.settings.gyro.scale = 720; // Set gyro range to +/-720dps
  imu.settings.mag.scale = 2; // Set mag range to +/-2Gs

  if (!imu.begin()) {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Looping to infinity.");
    while (1);
  }
  // Create Data Manager
  dm = new DataManager();
  dm->setDataManagerIMU(imu);
  // Set up the Ultrasonic Sensors
  pinMode(TRIG_PIN_FRONT, OUTPUT);
  digitalWrite(TRIG_PIN_FRONT, LOW);
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  digitalWrite(TRIG_PIN_RIGHT, LOW);
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  digitalWrite(TRIG_PIN_LEFT, LOW);
  pinMode(TRIG_PIN_BACK, OUTPUT);
  digitalWrite(TRIG_PIN_BACK, LOW);

  // Create State Machine
  state = new DeviceState();
  
 
  
  Serial.begin(9600);

  // Move to READY
  state->transition();
}

void move(int speed_right, int speed_left, int ramp_step) {
  int diff_right = speed_right - motorRight.getCurrentSpeed();
  int diff_left = speed_left - motorLeft.getCurrentSpeed();

  // If we aren't changing the values, simply return
  if (diff_right == 0 && diff_left == 0) {
    return;
  }
  for (int i=1; i <= ramp_step; i++) {
      motorRight.drive(motorRight.getCurrentSpeed() + diff_right*i/ramp_step);
      motorLeft.drive(motorLeft.getCurrentSpeed() + diff_left*i/ramp_step);
      delay(10); //10 ms delay
  }
  motorRight.setCurrentSpeed(speed_right);
  motorLeft.setCurrentSpeed(speed_left);
}

void ramp_searching() {
  int sensor = 0;
  float ultrasonic_left = dm->getUsLeft();
  float ultrasonic_front = dm->getUsFront();
//  float ultrasonic_front = readSensor();
  float ultrasonic_back = dm->getUsBack();
  float cur_mz = dm->getMagZ();
  float new_mz = cur_mz;
  switch(state->current) {
  case RAMP_SEARCH:
    move(MAX_SPEED/2, MAX_SPEED/2, 10);
    Serial.println("Searching for ramp");
    Serial.println(ultrasonic_front);
    if (ultrasonic_front < (RAMP_DIST_X)) {
//      state->transition();
    }
    break;
  case RAMP_TURN:
      move(-1*MAX_SPEED/2, MAX_SPEED/2, 4);
      sensor = analogRead(IR_PIN);
      // Get below 50
      while(sensor > 50) {
        sensor = analogRead(IR_PIN);
        Serial.println(sensor);
      }

      while(sensor < 200) {
        sensor = analogRead(IR_PIN);
        Serial.println(sensor);
      }
      move(MAX_SPEED/2, MAX_SPEED/2, 1);
      state->transition();
    break;
    
  case RAMP_AHEAD:
    Serial.println("RAMP_AHEAD");
    move(MAX_SPEED, MAX_SPEED, 20);
//    level_mz = dm->getMagZ();
//    Serial.println(level_mz);
//    new_mz = cur_mz;
//    Serial.println(new_mz);
//    while(abs(level_mz - new_mz) < ramp_change) {
//      dm->updateMag();
//      new_mz = dm->getMagZ();
//    }
    state->transition();
    break;
  }
}

void ramp_moving() {
  float ramp_up_mz = dm->getMagZ();
  float new_mz = ramp_up_mz;
  switch(state->current) {
  case RAMP_UP:
    Serial.println("RAMP_UP");
    move(MAX_SPEED, MAX_SPEED, 20);
    ramp_up_mz = dm->getMagZ();
    new_mz = ramp_up_mz;
    Serial.println(new_mz);
    while((new_mz - ramp_up_mz) < 0.1) {
      dm->updateMag();
      new_mz = dm->getMagZ();
    }
    state->transition();
    state->transition();
    break;
  case RAMP_DOWN:
    Serial.println("RAMP_DOWN");
    move(30, 30, 1); // Brake on the way down
    while(abs(new_mz -level_mz) > 0.02) {
      dm->updateMag();
      new_mz = dm->getMagZ();
      Serial.println(new_mz);
      delay(100);
    }
    state->transition();
    break;
  }
}

void base_searching() {
  Serial.println("base search");
  move(MAX_SPEED/3, MAX_SPEED/3, 3);
  delay(500);
  float USFront = dm->getUsFront();
  while(USFront > 20) {
    dm->updateFrontUS();
  }
  move(0,0,1);
  Serial.println("Done");
  state->transition();
}
void loop() {
  // Update Sensor Values
  dm->update();
//  Serial.println("Hello");
//  float sensor = readSensor();
//  Serial.println(sensor);
  switch(state->current) {
    case READY:
      state->transition();
      break;
    case RAMP_SEARCH:
    case RAMP_TURN:
    case RAMP_AHEAD:
      ramp_searching();
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
  delay(60);
}