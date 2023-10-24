#include "MotorControl.hpp"
#include "Motor.hpp"
#include "PIDController.hpp"
#include "Encoder.hpp"

MotorControl::MotorControl(int enPin, int phPin):
    _motor(enPin, phPin),
    _pid(0, 0, 0, 0)
{
    setTargetAngle(0);
    enablePIDControl(false);
    setPWM(0);
}

MotorControl::~MotorControl(){}

void MotorControl::setTargetAngle(int angle) {
    _targetAngle = angle;
}

void MotorControl::enablePIDControl(bool enable) {
    _pidEnabled = enable;
}

void MotorControl::setSamplingTime(int samplingTime) {
    _pid.setSamplingTime(samplingTime);
}

void MotorControl::setDeadbandOffset(int offset) {
    _pid.setDeadbandOffset(offset);
}

void MotorControl::setPIDParams(double kp, double ki, double kd) {
    _pid.setPIDParams(kp, ki, kd);
}

void MotorControl::setPWM(int pwm) {
    _pwm = pwm;
}

void MotorControl::setCurrentPosition(int counts) {
    _currentPosition = counts;
} 

void MotorControl::run() {
    static unsigned long lastMillis = 0;
    unsigned long dt = millis() - lastMillis;
    if (_pidEnabled) {
        if (dt > 50) {
            // TODO: Convert _targetAngle to encoder position
            int targetCounts = _targetAngle * (COUNTS_PER_REV / DEG_PER_REV);
            setPWM(_pid.computeControlSignal(targetCounts, _currentPosition));

            lastMillis = millis();
        }
    }

    if (_pwm > 0) _motor.setDirection(CW);
    else if (_pwm < 0) _motor.setDirection(CCW);
    else _motor.setDirection(OFF);

    _motor.setPWM(abs(_pwm));
    _motor.drive();    
}