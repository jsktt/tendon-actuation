#include "Motor.hpp"
#include "Arduino.h"

Motor::Motor(int enPin, int phPin) {
  _enPin = enPin;
  _phPin = phPin;

  pinMode(_enPin, INPUT);
  pinMode(_phPin, INPUT);

  setDirection(OFF);
  setPWM(0);
  drive();
}

Motor::~Motor(){}

void Motor::setDirection(MOTOR_DIR dir) {
  _dir = dir;
}

void Motor::setPWM(int pwm) {
  _pwm = pwm;
}

void Motor::drive() {
   switch (_dir) {
    case CCW:
      digitalWrite(_phPin, HIGH);
      analogWrite(_enPin, _pwm);
      break;
    case CW:
      digitalWrite(_phPin, LOW);
      analogWrite(_enPin, _pwm);
      break;
    case OFF:
      digitalWrite(_enPin, LOW);
      break;
   }
}