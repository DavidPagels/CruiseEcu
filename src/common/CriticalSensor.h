#ifndef CriticalSensor_h
#define CriticalSensor_h
#include "Sensor.h"

class CriticalSensor: public Sensor {
public:
    void update();
    bool isSafe();
private:
    int _lastMessageMs = 0;
};
#endif