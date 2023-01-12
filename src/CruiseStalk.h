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
    bool getSetPressed();
    bool getResPressed();
private:
    int _onOffPin;
    int _setResPin;
    int _ledPin;
    DebouncedInput _onOff = DebouncedInput(1000);
    DebouncedInput _set = DebouncedInput(200);
    DebouncedInput _res = DebouncedInput(200);
    bool _cruiseOn = false;
    bool _debounce(bool lastPressed, bool currentPressed, int startMs);
};
#endif