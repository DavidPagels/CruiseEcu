#include "SpeedSensor.h"

void SpeedSensor::update() {
    CriticalSensor::update();

    uint8_t speed_msg[8];
    getCanMessage(speed_msg, 8);
    _currentSpeed = ((speed_msg[0] * 256. + speed_msg[1]) / 100.);
    Serial.print(">rawSpeed:");
    Serial.println(_currentSpeed);
}

void SpeedSensor::updateLowRes() {
    uint8_t speed_msg[8];
    getCanMessage(speed_msg, 8);
    _currentLowResMs = millis();
    _currentLowResSpeed = speed_msg[2];
}

void SpeedSensor::writeToCan() {
  uint16_t kmh = (_currentLowResSpeed * 100);
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

double SpeedSensor::getCurrentSpeed() {
  double mid = _currentSpeed * 2 / 3.;
  double low = _currentSpeed / 3.;
  
  // Find the current state of the speed sensor
  double cleanSpeed;
  if (abs(_currentLowResSpeed - low) < abs(_currentLowResSpeed - mid)) {
    cleanSpeed = low;
  } else if (abs(_currentLowResSpeed - mid) < abs(_currentLowResSpeed - _currentSpeed)) {
    cleanSpeed = mid;
  } else {
    cleanSpeed = _currentSpeed;
  }

  // If the sensor is transitioning ranges, just use the low res speed
  double retSpeed = abs(_currentLowResSpeed - cleanSpeed) > (2 + 2 * (millis() - _currentLowResMs) / 500) && _currentLowResSpeed > 7 ? _currentLowResSpeed : cleanSpeed;
  Serial.print(millis());
  Serial.print(",");
  Serial.print(_currentSpeed);
  Serial.print(",");
  Serial.print(retSpeed);
  Serial.print(",");
  Serial.println(_currentLowResSpeed);
  return retSpeed;
}
