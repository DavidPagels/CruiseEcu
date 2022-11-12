#ifndef CruiseStalk_h
#define CruiseStalk_h
#include "common/Sensor.h"
#include "common/DebouncedInput.h"
#include <Arduino.h>

class CruiseStalk: public Sensor {
public:
    CruiseStalk(int onOffPin, int setResPin, int ledPin);
    void begin();
    void update();
    bool getCruiseOn();
    int getSetPressedMs();
    int getResPressedMs();
private:
    int _onOffPin;
    int _setResPin;
    int _ledPin;
    DebouncedInput _onOff = DebouncedInput();
    DebouncedInput _set = DebouncedInput();
    DebouncedInput _res = DebouncedInput();
    bool _cruiseOn = false;
    bool _debounce(bool lastPressed, bool currentPressed, int startMs);
};
#endif