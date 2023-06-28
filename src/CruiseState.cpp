#include "CruiseState.h"

CruiseState::CruiseState(CruiseStalk *cruiseStalk) {
  // p, i, d, hz, bits, signed
  _pid = new FastPID(255., 0., 0., 100, 16, false);
  _pid->setOutputRange(0., 600.);
}

void CruiseState::activate(int setSpeed, double currentThrottle) {
  _setSpeed = (float)setSpeed;
  _cruiseActive = true;
  _throttle = currentThrottle;
}

void CruiseState::resume(double currentThrottle) {
  if (_setSpeed > 0) {
    _cruiseActive = true;
    _throttle = currentThrottle;
  }
}

void CruiseState::deactivate() {
  _cruiseActive = false;
  _throttle = 0;
}

void CruiseState::writeToCan() {
  uint8_t pcmCruise[8] = {0x0};
  pcmCruise[0] = (_cruiseActive << 5) & 0x20 | 0x10;
  pcmCruise[6] = (_cruiseActive << 7) & 0x80;
  pcmCruise[7] = canChecksum(pcmCruise, 7, 0x1d2);
  writeCanMessage(0x1d2, pcmCruise);

  uint8_t pcmCruise2[8] = {0x0};
  pcmCruise2[1] = (_cruiseStalk->getCruiseOn() << 7) & 0x80 | 0x28;
  pcmCruise2[2] = _setSpeed;
  pcmCruise2[7] = canChecksum(pcmCruise2, 7, 0x1d3);
  writeCanMessage(0x1d3, pcmCruise2);
}

double CruiseState::updateThrottle(double speed) {
  if (!(_cruiseStalk->getCruiseOn() && _cruiseActive)) {
    return 0.;
  }
  int16_t pidThrottle = 0;
  // if (speed < _setSpeed * 0.85) {
  //    pidThrottle = 600;
  //  _pid.clear();
  //} else {
  pidThrottle = _pid->step(_setSpeed, speed);
  //}

  // Limit to 0.6. Since max is 600, divide by 1000
  _throttle = pidThrottle / 1000.;
  // Serial.print(millis());
  // Serial.print(" ");
  // Serial.print(_setSpeed);
  // Serial.print(" ");
  // Serial.print(speed);
  // Serial.print(" ");
  // Serial.println(_throttle);
  return _throttle;
}

bool CruiseState::getCruiseActive() { return _cruiseActive; }

void CruiseState::incrementSetSpeed() {
  int newSpeed = _setSpeed + round(1.60934);
  // Limit to ~80mph
  _setSpeed = min(newSpeed, 130);
}

void CruiseState::decrementSetSpeed() {
  int newSpeed = _setSpeed - round(1.60934);
  // Limit to ~18mph
  _setSpeed = max(newSpeed, 28);
}

void CruiseState::clearSetSpeed() { _setSpeed = 0.; }
