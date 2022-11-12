#include "SpeedSensor.h"

void SpeedSensor::update() {
    CriticalSensor::update();

    uint8_t speed_msg[8];
    getCanMessage(speed_msg, 8);
    _currentSpeed = (speed_msg[0] * 256. + speed_msg[1]);
}

void SpeedSensor::writeToCan() {
  uint16_t kmh = (_currentSpeed * 100);
  uint8_t speedPacket[8];
  speedPacket[5] = (kmh >> 8) & 0xFF;
  speedPacket[6] = kmh & 0xFF;
  speedPacket[7] = canChecksum(speedPacket, 7, 0xb4);
  writeCanMessage(0xb4, speedPacket);

  uint8_t dat[8];
  uint16_t wheelspeed = 0x1a6f + kmh;
  dat[0] = (wheelspeed >> 8) & 0xFF;
  dat[1] = (wheelspeed >> 0) & 0xFF;
  dat[2] = (wheelspeed >> 8) & 0xFF;
  dat[3] = (wheelspeed >> 0) & 0xFF;
  dat[4] = (wheelspeed >> 8) & 0xFF;
  dat[5] = (wheelspeed >> 0) & 0xFF;
  dat[6] = (wheelspeed >> 8) & 0xFF;
  dat[7] = (wheelspeed >> 0) & 0xFF;
  writeCanMessage(0xaa, dat);
}

float SpeedSensor::getCurrentSpeed() {
  return _currentSpeed;
}
