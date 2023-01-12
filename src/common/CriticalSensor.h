#ifndef CriticalSensor_h
#define CriticalSensor_h
#include "Sensor.h"

class CriticalSensor: public Sensor {
public:
    CriticalSensor(int safeMs);
    void update();
    bool isSafe();
private:
    unsigned long _lastMessageMs = 0;
    int _safeMs = 0;
};
#endif