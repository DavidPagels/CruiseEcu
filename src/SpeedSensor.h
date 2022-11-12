#ifndef SpeedSensor_h
#define SpeedSensor_h
#include "common/CriticalSensor.h"
#include "common/CanDevice.h"

class SpeedSensor: public CriticalSensor, CanDevice {
public:
    void update();
    void writeToCan();
    float getCurrentSpeed();
private:
    float _currentSpeed = 0.;
};
#endif