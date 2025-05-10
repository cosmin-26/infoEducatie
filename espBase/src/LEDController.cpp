#include "LEDController.h"

LEDController::LEDController(uint8_t *ledPin, bool usePWM, uint8_t pwmChannel)
{
    size = sizeof(&ledPin) / (uint8_t)(1);
    pin = ledPin;
    isPWM = usePWM;
    channel = pwmChannel;
    state = LOW;
    brightness = 0;

    if (isPWM)
    {
        // Set up PWM if enabled
        ledcSetup(channel, 5000, 8); // 5kHz frequency, 8-bit resolution
        for (size_t i = 0; i < size; i++)
        {
            ledcAttachPin(pin[i], channel); // Attach pin to PWM channel
        }
    }
    else
    {
        for (size_t i = 0; i < size; i++)
        {
            pinMode(pin[i], OUTPUT);
            digitalWrite(pin[i], state);
        }
    }
}

// Digital on/off functions
void LEDController::on()
{
    if (isPWM)
    {
        setBrightness(255);
    }
    else
    {
        state = HIGH;
        for (size_t i = 0; i < size; i++)
            digitalWrite(pin[i], state);
    }
}

void LEDController::off()
{
    if (isPWM)
    {
        setBrightness(0);
    }
    else
    {
        state = LOW;
        for (size_t i = 0; i < size; i++)
            digitalWrite(pin[i], state);
    }
}

void LEDController::toggle()
{
    if (isPWM)
    {
        brightness = (brightness == 0) ? 255 : 0;
        setBrightness(brightness);
    }
    else
    {
        state = !state;
        for (size_t i = 0; i < size; i++)
            digitalWrite(pin[i], state);
    }
}

void LEDController::blink(unsigned int delayTime)
{
    on();
    delay(delayTime);
    off();
    delay(delayTime);
}

// PWM functions
void LEDController::setBrightness(uint16_t value)
{
    if (isPWM)
    {
        brightness = constrain(value, 0, 255);
        ledcWrite(channel, brightness);
    }
}

void LEDController::fadeIn(uint16_t stepDelay)
{
    if (isPWM)
    {
        for (int i = 0; i <= 255; i++)
        {
            setBrightness(i);
            delay(stepDelay);
        }
    }
}

void LEDController::fadeOut(uint16_t stepDelay)
{
    if (isPWM)
    {
        for (int i = 255; i >= 0; i--)
        {
            setBrightness(i);
            delay(stepDelay);
        }
    }
}
