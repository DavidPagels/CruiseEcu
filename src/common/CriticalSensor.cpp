#include "CriticalSensor.h"

void CriticalSensor::update() {
    _lastMessageMs = millis();
}

bool CriticalSensor::isSafe() {
    return millis() - _lastMessageMs < 1500;
}