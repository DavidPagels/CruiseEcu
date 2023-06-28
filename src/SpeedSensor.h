#ifndef SpeedSensor_h
#define SpeedSensor_h
#include "EngineRpm.h"
#include "common/CanDevice.h"
#include "common/CriticalSensor.h"

class SpeedSensor : public CriticalSensor, CanDevice {
 public:
  static const int SAFE_MS = 100;
  SpeedSensor() : CriticalSensor(SAFE_MS){};
  void update();
  void updateMph();
  void writeToCan();
  double getCurrentSpeed();

 private:
  double _currentSpeed = 0.;
  uint8_t _currentMphSpeed = 0;
  unsigned long _currentMphMs = 0;
};
#endif