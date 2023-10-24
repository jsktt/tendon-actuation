#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#define OUTPUT_MAX      255
#define OUTPUT_MIN      -255

class PIDController
{
    public:
        PIDController(double kp, double ki, double kd, int samplingTime);
        ~PIDController();
        void setPIDParams(double kp, double ki, double kd);
        void setSamplingTime(int samplingTime);
        void setDeadbandOffset(int offset);
        int computeControlSignal(int setPoint, int currentPosition);
    private:
        double _kp;
        double _ki;
        double _kd;
        double _integral;
        int _samplingTime;
        int _deadbandOffset;

};

#endif