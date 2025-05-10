#include "Button.h"

Button::Button(/* args */) : _pin(-1)
{
}

Button::Button(uint8_t pin) : _pin(pin)
{
    pinMode(_pin, INPUT_PULLUP);
}

Button::~Button()
{
}

void Button::setPin(uint8_t pin)
{
    _pin = pin;
}

bool Button::check()
{
    _currentState = !(digitalRead(_pin));
    return _currentState;
}

bool Button::checkToggle()
{
    _currentState = !digitalRead(_pin);

    if (!_lastState && _currentState && !_waitingForRelease)
    {
        _waitingForRelease = true;
        _lastState = _currentState;
        return true;
    }

    // Reset when fully released
    if (_waitingForRelease && !_currentState)
    {
        _waitingForRelease = false;
    }

    _lastState = _currentState;
    return false;
}

bool Button::checkLongPress(unsigned long durationMs)
{
    _currentState = !digitalRead(_pin);

    if (_currentState)
    {
        if (_pressedTime == 0)
        {
            _pressedTime = millis();
        }
        if (!_isLongPressDetected && millis() - _pressedTime >= durationMs)
        {
            _isLongPressDetected = true;
            return true;
        }
    }
    else
    {
        _pressedTime = 0;
        _isLongPressDetected = false;
    }
    return false;
}