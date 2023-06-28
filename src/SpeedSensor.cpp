#include "SpeedSensor.h"

void SpeedSensor::update() {
  CriticalSensor::update();

  uint8_t speed_msg[8];
  getCanMessage(speed_msg, 8);
  uint16_t rawSpeed = _currentSpeed = speed_msg[0] * 256. + speed_msg[1];
  uint8_t mult = speed_msg[4] & 0x03;
  _currentSpeed = rawSpeed * mult * 0.004933;
  Serial.print(">rawSpeed:");
  Serial.println(_currentSpeed);
}

void SpeedSensor::updateMph() {
  uint8_t speed_msg[8];
  getCanMessage(speed_msg, 8);
  _currentMphMs = millis();
  _currentMphSpeed = speed_msg[2];
}

void SpeedSensor::writeToCan() {
  uint16_t mph = (_currentMphSpeed * 100);
  uint8_t speedPacket[8];
  speedPacket[5] = (mph >> 8) & 0xFF;
  speedPacket[6] = mph & 0xFF;
  speedPacket[7] = canChecksum(speedPacket, 7, 0xb4);
  writeCanMessage(0xb4, speedPacket);

  uint8_t dat[8];
  uint16_t wheelspeed = 0x1a6f + mph;
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
  // double mid = _currentSpeed * 2 / 3.;
  // double low = _currentSpeed / 3.;

  // // Find the current state of the speed sensor
  // double cleanSpeed;
  // if (abs(_currentMphSpeed - low) < abs(_currentMphSpeed - mid)) {
  //   cleanSpeed = low;
  // } else if (abs(_currentMphSpeed - mid) <
  //            abs(_currentMphSpeed - _currentSpeed)) {
  //   cleanSpeed = mid;
  // } else {
  //   cleanSpeed = _currentSpeed;
  // }
  double lowResKph = _currentMphSpeed * 1.60934;

  // If the sensor is transitioning ranges, just use the low res speed
  double retSpeed = abs(lowResKph - _currentSpeed) >
                            (2 + 2 * (millis() - _currentMphMs) / 500)
                        ? lowResKph
                        : _currentSpeed;
  Serial.print(millis());
  Serial.print(",");
  Serial.print(_currentSpeed);
  Serial.print(",");
  Serial.print(retSpeed);
  Serial.print(",");
  Serial.println(lowResKph);
  return retSpeed;
}
