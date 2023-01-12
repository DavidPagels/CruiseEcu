#ifndef DebouncedInput_h
#define DebouncedInput_h
#include "Arduino.h" 

class DebouncedInput {
public:
    DebouncedInput(int toggleDelayMs);
    bool getPressed();
    void debounce(bool pressed);
private:
    bool _pressed = false;
    unsigned long _firstPressedMs = 0;
    bool _debouncedPressed = false;
    unsigned long _lastDebouncedPressed = 0;
    int _toggleDelayMs = 500;
    static const int DEBOUNCE_DELAY = 50;
};
#endif