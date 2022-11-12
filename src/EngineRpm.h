#ifndef EngineRpm_h
#define EngineRpm_h
#include "common/CriticalSensor.h"
#include "common/CanDevice.h"

class EngineRpm: public CriticalSensor, CanDevice {
public:
    void update();
    bool getEngineRpm();
private:
    int _engineRpm = 0;
};
#endif