#pragma once

#include "Arduino.h"

class Button
{
private:
    uint8_t _pin;
    bool _currentState, _lastState = HIGH;
    unsigned long _pressedTime = 0;
    bool _isLongPressDetected = false;
    bool _waitingForRelease = false;

public:
    Button();
    Button(uint8_t pin);
    ~Button();

    void setPin(uint8_t pin);
    bool check();
    bool checkToggle();
    bool checkLongPress(unsigned long durationMs);
};
