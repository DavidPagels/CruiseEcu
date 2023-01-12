#ifndef SpeedSensor_h
#define SpeedSensor_h
#include "common/CriticalSensor.h"
#include "common/CanDevice.h"

class SpeedSensor: public CriticalSensor, CanDevice {
public:
    static const int SAFE_MS = 100;
    SpeedSensor(): CriticalSensor(SAFE_MS) {};
    void update();
    void writeToCan();
    double getCurrentSpeed();
private:
    double _currentSpeed = 0.;
};
#endif