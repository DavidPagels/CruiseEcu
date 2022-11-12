#ifndef TurnLevers_h
#define TurnLevers_h
#include "common/Sensor.h"
#include "common/CanDevice.h"
#include <Arduino.h>

class TurnLevers: public Sensor, CanDevice {
public:
    TurnLevers(int leftLeverPin, int rightLeverPin);
    void begin();
    void update();
    void writeToCan();
private:
    int _leftLeverPin;
    int _rightLeverPin;
    bool _leftLeverActive;
    bool _rightLeverActive;
};
#endif
