#pragma once

#include <Arduino.h>

class LEDController
{
private:
    uint8_t size = 1; // allows for multiple leds
    uint8_t *pin;
    uint8_t channel;     // PWM channel (for brightness control)
    bool isPWM;          // Indicates if PWM is used
    bool state;          // LED on/off state
    uint16_t brightness; // PWM brightness (0-255)

public:
    LEDController(uint8_t *ledPin, bool usePWM = false, uint8_t pwmChannel = 0);

    // Digital control
    void on();
    void off();
    void toggle();
    void blink(unsigned int delayTime);

    // PWM control
    void setBrightness(uint16_t value);
    void fadeIn(uint16_t stepDelay);
    void fadeOut(uint16_t stepDelay);
};

struct LED
{
    int n;
    int *pins;

    LED(int n, int *pins) : n(n), pins(pins)
    {
        for (int i = 0; i < n; i++)
            pinMode(pins[i], OUTPUT);
    }

    void setPower(size_t power)
    {
        for (size_t i = 0; i < n; i++)
        {
            digitalWrite(pins[i], power);
        }
    }
};
