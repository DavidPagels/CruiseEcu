#include "Brake.h"

void Brake::update() {
    CriticalSensor::update();
    uint8_t brake_msg[8];
    getCanMessage(brake_msg, 8);
    _brakePressed = ((brake_msg[4] & 0x01) == 1);
}

void Brake::writeToCan() {
  uint8_t dat[8] = {0x0};
  dat[0] = (_brakePressed << 5) & 0x20;
  writeCanMessage(0x224, dat);
}

bool Brake::getBrakePressed() {
  return _brakePressed;
}
