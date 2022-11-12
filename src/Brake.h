#ifndef Brake_h
#define Brake_h
#include "common/CriticalSensor.h"
#include "common/CanDevice.h"

class Brake: public CriticalSensor, CanDevice {
public:
    void update();
    void writeToCan();
    bool getBrakePressed();
private:
    bool _brakePressed = true;
};
#endif