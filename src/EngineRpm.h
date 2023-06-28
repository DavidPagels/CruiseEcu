#ifndef EngineRpm_h
#define EngineRpm_h
#include "common/CanDevice.h"
#include "common/CriticalSensor.h"

class EngineRpm : public CriticalSensor, CanDevice {
 public:
  static const int SAFE_MS = 100;
  EngineRpm() : CriticalSensor(SAFE_MS){};
  void update();
  int getEngineRpm();

 private:
  int _engineRpm = 0;
};
#endif