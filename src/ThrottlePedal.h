#ifndef ThrottlePedal_h
#define ThrottlePedal_h
#include "common/Sensor.h"
#include "common/CanDevice.h"
#include <Arduino.h>

class ThrottlePedal: public Sensor, CanDevice {
public:
    ThrottlePedal(int pedalPin, int throttlePin, float offset);
    void begin();
    void update();
    void writeToCan();
    float getPedalPosition();
    float getThrottlePosition();
    void setThrottlePosition(float position);
private:
    int _pedalPin;
    int _throttlePin;
    float _offset;
    float _pedalPosition;
    float _throttlePosition;
};
#endif