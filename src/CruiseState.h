#ifndef CruiseState_h
#define CruiseState_h
#include <FastPID.h>

#include "CruiseStalk.h"
#include "common/CanDevice.h"

class CruiseState : public CanDevice {
 public:
  CruiseState(CruiseStalk *cruiseStalk);
  void activate(int setSpeed, double currentThrottle);
  void resume(double currentThrottle);
  void deactivate();
  void writeToCan();
  double updateThrottle(double speed);
  bool getCruiseActive();
  void incrementSetSpeed();
  void decrementSetSpeed();
  void clearSetSpeed();

 private:
  FastPID *_pid;
  float _setSpeed = 0;
  double _throttle = 0.;
  bool _cruiseActive = false;
  CruiseStalk *_cruiseStalk;
};
#endif