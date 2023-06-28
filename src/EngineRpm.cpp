#include "EngineRpm.h"

void EngineRpm::update() {
  CriticalSensor::update();

  uint8_t rpm_msg[8];
  getCanMessage(rpm_msg, 8);
  _engineRpm = (rpm_msg[0] * 256. + rpm_msg[1]) * 0.78125;
}

int EngineRpm::getEngineRpm() { return _engineRpm; }
