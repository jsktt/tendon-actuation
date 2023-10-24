#include <Arduino.h>
#include <MotorControl.hpp>
#include <Encoder.hpp>
#include <SPI_slave.hpp>

#define EN1   2
#define PH1   3
#define ENCA1 24
#define ENCB1 25

#define EN2   4
#define PH2   5
#define ENCA2 22
#define ENCB2 23

// #define EN3   4
// #define PH3   5
// #define ENCA3 3 // Yellow
// #define ENCB3 2  // White

// #define EN4   4
// #define PH4   5
// #define ENCA4 3 // Yellow
// #define ENCB4 2  // White

// #define EN5   4
// #define PH5   5
// #define ENCA5 3 // Yellow
// #define ENCB5 2  // White

// #define EN6   4
// #define PH6   5
// #define ENCA7 3 // Yellow
// #define ENCB7 2  // White

// #define EN8   4
// #define PH8   5
// #define ENCA8 3 // Yellow
// #define ENCB8 2  // White
// #define EN1   4
// #define PH1   5
// #define ENCA1 3 // Yellow
// #define ENCB1 2  // White

#define K_P   1
#define K_I   0.25
#define K_D   0.5

Encoder<ENCA1, ENCB1> enc1;
Encoder<ENCA2, ENCB2> enc2;
// Encoder<ENCA1, ENCB1> enc2;
// Encoder<ENCA1, ENCB1> enc3;
// Encoder<ENCA1, ENCB1> enc4;
// Encoder<ENCA1, ENCB1> enc5;
// Encoder<ENCA1, ENCB1> enc6;
// Encoder<ENCA1, ENCB1> enc7;
// Encoder<ENCA1, ENCB1> enc8;
// Encoder<ENCA1, ENCB1> enc9;
// Encoder<ENCA1, ENCB1> enc10;

MotorControl m1 = MotorControl(EN1, PH1);
MotorControl m2 = MotorControl(EN2, PH2);
// MotorControl m3 = MotorControl(EN1, PH1);
// MotorControl m4 = MotorControl(EN1, PH1);
// MotorControl m5 = MotorControl(EN1, PH1);
// MotorControl m6 = MotorControl(EN1, PH1);
// MotorControl m7 = MotorControl(EN1, PH1);
// MotorControl m8 = MotorControl(EN1, PH1);
// MotorControl m9 = MotorControl(EN1, PH1);
// MotorControl m10 = MotorControl(EN1, PH1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // pinMode(SS, INPUT_PULLUP);
  // pinMode(MOSI, INPUT);

  initSPI();

  // // Run Motors without PID - PID disabled by default
  // m1.setPWM(150);   // Can set negative pwm to move in CCW rotation
  // m2.setPWM(150);

  // // Run Motors with PID
  m1.enablePIDControl(true);   // Enable PID
  m1.setPIDParams(K_P, K_I, K_D);   // Set PID parameters
  m1.setSamplingTime(50);
  m1.setDeadbandOffset(0); // - Optional
  m1.setTargetAngle(180);   // Set target angle here
  enc1.resetCount();   // Make sure to reset count
  m1.setCurrentPosition(enc1.getTicks());
  enc1.init();  // Initialize encoders

  m2.enablePIDControl(true);
  m2.setPIDParams(K_P, K_I, K_D);
  m2.setSamplingTime(50);
  m2.setDeadbandOffset(0);
  m2.setTargetAngle(180);
  enc2.resetCount();
  m2.setCurrentPosition(enc2.getTicks());
  enc2.init();
}


void loop() {
  // If running motors with PID you must update currentPosition each loop
   m1.setCurrentPosition(enc1.getTicks());
   m1.run();
   m2.setCurrentPosition(enc2.getTicks());
   m2.run();
}