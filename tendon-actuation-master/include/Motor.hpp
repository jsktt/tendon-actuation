#ifndef MOTOR_H
#define MOTOR_H

typedef enum {
    CW, CCW, OFF
} MOTOR_DIR;

class Motor
{
    public:
        Motor(int enPin, int phPin);
        ~Motor();
        void setPWM(int pwm);
        void setDirection(MOTOR_DIR dir);
        void drive();
    private:
        int _enPin;
        int _phPin;
        int _pwm;
        MOTOR_DIR _dir;
};

#endif