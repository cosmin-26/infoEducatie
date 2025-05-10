#pragma once

#include <HX711.h>

class Scale
{
public:
    Scale(uint8_t dataPin, uint8_t clockPin);
    void begin(bool calibrate = false, float knownWeightGrams = 0);
    float getWeight(uint8_t samples = 5);
    float getScaleFactor() const;

private:
    HX711 scale;
    float scaleFactor;
    uint8_t _dataPin, _clockPin;

    long getStableReading(uint8_t samples);
};
