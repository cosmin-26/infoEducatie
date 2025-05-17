#include "Scale.h"
#include <Arduino.h>

Scale::Scale(uint8_t dataPin, uint8_t clockPin)
    : _dataPin(dataPin), _clockPin(clockPin), scaleFactor(1.0f) {}

void Scale::begin(bool calibrate, float knownWeightGrams)
{
    scale.begin(_dataPin, _clockPin);
    delay(500);

    Serial.println("📏 Taring scale...");
    scale.tare();
    delay(1000);
    Serial.println("✅ Tare complete.");

    if (calibrate && knownWeightGrams > 0.0f)
    {
        Serial.println("🧪 Calibration started. Do NOT place any weight...");
        delay(3000);

        long baseline = getStableReading(10);
        Serial.println("Now place the known weight...");
        delay(6000);

        long reading = getStableReading(10);
        long diff = reading - baseline;
        scaleFactor = (float)diff / knownWeightGrams;

        Serial.print("🧮 Calibration done. Factor: ");
        Serial.println(scaleFactor, 4);
    }
    else
    {
        // Set your pre-determined scale factor here if not calibrating
        scaleFactor = -1043.6256f; // 🔁 Replace with your saved value
    }

    scale.set_scale(scaleFactor);
}

float Scale::getWeight(uint8_t samples)
{
    return scale.get_units(samples);
}

long Scale::getStableReading(uint8_t samples)
{
    long sum = 0;
    for (uint8_t i = 0; i < samples; ++i)
    {
        sum += scale.read();
        delay(100);
    }
    return sum / samples;
}

float Scale::getScaleFactor() const
{
    return scaleFactor;
}