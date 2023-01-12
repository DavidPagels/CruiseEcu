#ifndef ThrottlePedal_h
#define ThrottlePedal_h
#include "common/Sensor.h"
#include "common/CanDevice.h"
#include <Arduino.h>

class ThrottlePedal: public Sensor, CanDevice {
public:
    ThrottlePedal(int pedalPin, int throttlePin, double offset);
    void begin();
    void update();
    void writeToCan();
    double getPedalPosition();
    double getThrottlePosition();
    void setThrottlePosition(double position);
private:
    int _pedalPin;
    int _throttlePin;
    double _offset;
    double _pedalPosition;
    double _throttlePosition;
};
#endif