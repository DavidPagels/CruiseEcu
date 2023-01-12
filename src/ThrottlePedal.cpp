#include "ThrottlePedal.h"

ThrottlePedal::ThrottlePedal(int pedalPin, int throttlePin, double offset) {
	_pedalPin = pedalPin;
	_throttlePin = throttlePin;
	_offset = offset;
};

void ThrottlePedal::begin() {
	pinMode(_pedalPin, INPUT);
	pinMode(_throttlePin, OUTPUT);
	analogWrite(_throttlePin, 0);
}

void ThrottlePedal::update() {
	int pedalPosition = analogRead(_pedalPin);
	_pedalPosition = (5. * pedalPosition / 1023.) - _offset;
}

void ThrottlePedal::writeToCan() {
	uint8_t pedalPosition[8] = {0x0};
	pedalPosition[0] = 0x08;
	writeCanMessage(0x2c2, pedalPosition);
}

double ThrottlePedal::getPedalPosition(){
	return _pedalPosition;
}

double ThrottlePedal::getThrottlePosition(){
	return _throttlePosition - _offset;
}

void ThrottlePedal::setThrottlePosition(double position) {
	// 255
	_throttlePosition = position + _offset;
	analogWrite(_throttlePin, _throttlePosition / 5. * 255);
}