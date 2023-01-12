#ifndef Brake_h
#define Brake_h
#include "common/CriticalSensor.h"
#include "common/CanDevice.h"

class Brake: public CriticalSensor, CanDevice {
public:
    static const int SAFE_MS = 1100;
    Brake(): CriticalSensor(SAFE_MS) {};
    void update();
    void writeToCan();
    bool getBrakePressed();
private:
    bool _brakePressed = true;
};
#endif