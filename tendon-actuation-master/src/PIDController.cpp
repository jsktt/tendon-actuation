#include "PIDController.hpp"
#include "Arduino.h"

PIDController::PIDController(double kp, double ki, double kd, int samplingTime) {
    setPIDParams(kp, ki, kd);
    setSamplingTime(samplingTime);
    setDeadbandOffset(0);
}

PIDController::~PIDController(){}

void PIDController::setPIDParams(double kp, double ki, double kd) {
    _kp = kp;
    _ki = ki;
    _kd = kd;
    _integral = 0;
}

void PIDController::setSamplingTime(int samplingTime) {
    _samplingTime = samplingTime;
}

void PIDController::setDeadbandOffset(int offset) {
    _deadbandOffset = offset;
}

int PIDController::computeControlSignal(int setPoint, int currentPosition) {
    static unsigned long _lastMillis = millis();
    static int _prevError = 0;

    int pid = 0;

    unsigned long _dt = millis() - _lastMillis;
    if (_dt >= (unsigned long)_samplingTime) {
        double p_error = (setPoint - currentPosition);
        double d_error = (p_error - _prevError);
        _integral += p_error;
        _prevError = p_error;

        pid = (_kp * p_error) + (_ki * _integral) + (_kd * d_error);

        if (p_error > 0) {
            pid += _deadbandOffset;
        }
        else {
            pid -= _deadbandOffset;
        }

        pid = constrain(pid, OUTPUT_MIN, OUTPUT_MAX);
        _lastMillis = millis();
    }

    return pid;
}