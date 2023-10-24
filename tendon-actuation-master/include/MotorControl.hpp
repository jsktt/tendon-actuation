#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "Motor.hpp"
#include "Encoder.hpp"
#include "PIDController.hpp"

class MotorControl {
    public:
        MotorControl(int enPin, int phPin);
        ~MotorControl();
        void setTargetAngle(int angle);
        void enablePIDControl(bool enable);
        void setSamplingTime(int samplingTime);
        void setDeadbandOffset(int offset);
        void setPIDParams(double kp, double ki, double kd);
        void setCurrentPosition(int count);
        void setPWM(int pwm);
        void run();

    private:
        Motor _motor;
        PIDController _pid;
        int _targetAngle;
        bool _pidEnabled;
        int _samplingTime;
        int _deadbandOffset;
        int _pwm;
        int _currentPosition;
};

#endif