#include "DebouncedInput.h"

bool DebouncedInput::debounce(bool pressed) {
    bool pressedDebounced = false;
    if (pressed && pressed != _pressed) {
        _durationPressedMs = millis();
    }
    _pressed = pressed;
    _debouncedPressed = pressed && millis() - _durationPressedMs >= DEBOUNCE_DELAY;
    return _debouncedPressed;
}

int DebouncedInput::getPressedMs() {
    return _pressed && _durationPressedMs >= DEBOUNCE_DELAY ? _durationPressedMs - DEBOUNCE_DELAY : 0;
}

