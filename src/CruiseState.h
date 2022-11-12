#ifndef CruiseState_h
#define CruiseState_h
#include "common/CanDevice.h"
#include "CruiseStalk.h"

class CruiseState: public CanDevice {
public:
    CruiseState(CruiseStalk &cruiseStalk);
    void activate(int setSpeed, float currentThrottle);
    void deactivate();
    void writeToCan();
    float updateThrottle(float speed);
    bool getCruiseActive();
    void incrementSetSpeed();
    void decrementSetSpeed();
    void clearSetSpeed();
private:
    int _setSpeed = 0;
    float _throttle = 0.;
    bool _cruiseActive = false;
    CruiseStalk _cruiseStalk;
};
#endif