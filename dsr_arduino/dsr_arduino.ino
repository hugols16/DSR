#include <Wire.h>
#include <SPI.h>

#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include <SparkFunLSM9DS1.h>

#include <SparkFun_TB6612.h>

#include "source/Defines.h"
#include "source/DeviceState.cpp"
#include "source/SensorReading.cpp"

// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Create motor objects
Motor motorRight = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motorLeft = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Create IMU object
LSM9DS1 imu;

// For testing
volatile float dist = 0;

// Current (x,y) positions
volatile int x_pos = 0;
volatile int y_pos = 0;

// Expected (x,y) positions
volatile int x_exp = 0;
volatile int y_exp = 0;

// Create Global State Machine
DeviceState* state;

//Might want to put the following in the Defines
// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 400.0;

// Starting x distance from the ramp (exact distance to middle)
const unsigned int RAMP_DIST_X = 35.0;

// Starting y distance from the ramp (exact distance to bottom)
const unsigned int RAMP_DIST_Y = 35.0;

void setup() {

  // Setting up the imu
  imu.settings.device.commInterface = IMU_MODE_I2C; // Set mode to I2C
  imu.settings.device.mAddress = LSM9DS1_M; // Set mag address to 0x1E
  imu.settings.device.agAddress = LSM9DS1_AG; // Set ag address to 0x6B
  imu.begin();

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

void ramp_searching() {
  float ultrasonic_left = sensorReadingLeft();
  Serial.println("Searching for ramp");
  if (ultrasonic_left < (RAMP_DIST_Y + 10) || x_pos >= RAMP_DIST_X) {
    // rotate 90 degrees to the left
    while(1){
      Serial.println("Turning");  
    }
//    motorLeft.brake();
//    motorRight.drive(MAX_SPEED/2.0);
    //while(not 90 degree turn){}

    // Move straight
    //motor1.drive(MAX_SPEED);
    //motor2.drive(MAX_SPEED);
    
    // drive to the ramp BY RAMP_DIST_X centimeters
    //while (y_dist < RAMP_DIST_Y) {}
    
    // Change states
    //state->transition();
  } else {
//    motorRight.drive(MAX_SPEED/2.0);
//    motorLeft.drive(MAX_SPEED/2.0);
  }
}
void loop() {
//  int sensorValue = analogRead(IR_PIN);
//  Serial.println(sensorValue);
  
  dist = sensorReadingFront();
//  Serial.print(state->current);
//  Serial.print(" \n");
  switch(state->current) {
    case READY:
      Serial.println("Ready State");
      if (dist < 20) {
        state->transition();
      }
      break;
    case RAMP_SEARCH:
      ramp_searching();
      break;
    case RAMP_UP:
      break;
    case RAMP_LEVEL:
      break;
    case RAMP_DOWN:
      break;
    case SEARCHING:
      break;
  }
  if ( dist > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(dist);
    Serial.print(" cm \n");
  }
//  imu.readMag(); // Update the accelerometer data
//  Serial.print(imu.calcMag(imu.mx)); // Print x-axis data
//  Serial.print(", ");
//  Serial.print(imu.calcMag(imu.my)); // print y-axis data
//  Serial.print(", ");
//  Serial.println(imu.calcMag(imu.mz)); // print z-axis data
  delay(60);
}
