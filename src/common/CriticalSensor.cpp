#include "CriticalSensor.h"

CriticalSensor::CriticalSensor(int safeMs) {
    _safeMs = safeMs;
}

void CriticalSensor::update() {
    _lastMessageMs = millis();
}

bool CriticalSensor::isSafe() {
    return millis() - _lastMessageMs < 1500;
}