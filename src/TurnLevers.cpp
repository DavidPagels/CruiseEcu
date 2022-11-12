#include "TurnLevers.h"

TurnLevers::TurnLevers(int leftLeverPin, int rightLeverPin) {
	_leftLeverPin = leftLeverPin;
	_rightLeverPin = rightLeverPin;
};

void TurnLevers::begin() {
	pinMode(_leftLeverPin, INPUT);
	pinMode(_rightLeverPin, INPUT);
}

void TurnLevers::update() {
	_leftLeverActive = digitalRead(_leftLeverPin);
	_rightLeverActive = digitalRead(_rightLeverPin);
}

void TurnLevers::writeToCan() {
  uint8_t dat[8] = {0x0};
  dat[3] = (_leftLeverActive << 5) & 0x20 | (_rightLeverActive << 4 & 0x10);
  writeCanMessage(0x614, dat);
}
