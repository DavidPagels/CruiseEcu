#include "CruiseStalk.h"

CruiseStalk::CruiseStalk(int onOffPin, int setResPin, int ledPin) {
    _onOffPin = onOffPin;
    _setResPin = setResPin;
    _ledPin = ledPin;
}

void CruiseStalk::begin() {
    pinMode(_onOffPin, INPUT);
    pinMode(_setResPin, INPUT);
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
}

void CruiseStalk::update() {
    int onOffPressed = analogRead(_onOffPin);
    int setResPressed = analogRead(_setResPin);
    _set.debounce(setResPressed > 800);
    _res.debounce(setResPressed < 800 && setResPressed > 500);
    _onOff.debounce(onOffPressed > 500);
    if (_onOff.getPressed()) {
        Serial.println("ONOFF Pressed");
        _cruiseOn = !_cruiseOn;
        digitalWrite(_ledPin, _cruiseOn);
    }
}

bool CruiseStalk::getCruiseOn() {
    return _cruiseOn;
}

bool CruiseStalk::getSetPressed() {
    return _set.getPressed();
}

bool CruiseStalk::getResPressed() {
    return _res.getPressed();
}
