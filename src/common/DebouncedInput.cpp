#include "DebouncedInput.h"

DebouncedInput::DebouncedInput(int toggleDelayMs) {
    _toggleDelayMs = toggleDelayMs;
}

void DebouncedInput::debounce(bool pressed) {
    bool pressedDebounced = false;
    if (pressed && pressed != _pressed) {
        _firstPressedMs = millis();
    }
    _pressed = pressed;
    _debouncedPressed = pressed && millis() - _firstPressedMs >= DEBOUNCE_DELAY && millis() - _lastDebouncedPressed >= _toggleDelayMs;
}

bool DebouncedInput::getPressed() {
    if (_debouncedPressed) {
        _lastDebouncedPressed = millis();
        return true;
    }
    return false;
}

