// Ultrasonic Pins
#define ECHO_PIN_RIGHT 31
#define TRIG_PIN_RIGHT 33
#define ECHO_PIN_FRONT 35
#define TRIG_PIN_FRONT 37
#define ECHO_PIN_LEFT 39
#define TRIG_PIN_LEFT 41
#define ECHO_PIN_BACK 43
#define TRIG_PIN_BACK 45

// IMU Pins
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M   0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// IR Pins
#define IR_PIN A15

// Motor Pins
#define AIN1 9
#define BIN1 11
#define AIN2 8
#define BIN2 12
#define PWMA 7
#define PWMB 13
#define STBY 10

// Motor Constants
#define MAX_SPEED 215
#define MOTOR_STANDBY 999

// Distances
#define RAMP_DIST_X 26.0f
#define RAMP_DIST_Y 45.0f

#define offsetA 1
#define offsetB 1

#define TURN_TOLERANCE 0.1
#define RIGHT 0
#define LEFT 1
