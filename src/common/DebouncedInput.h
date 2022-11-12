#ifndef DebouncedInput_h
#define DebouncedInput_h
#include "Arduino.h" 

class DebouncedInput {
public:
    int getPressedMs();
    bool debounce(bool pressed);
private:
    bool _pressed = false;
    bool _debouncedPressed = false;
    int _durationPressedMs = 0;
    static const int DEBOUNCE_DELAY = 50;
};
#endif