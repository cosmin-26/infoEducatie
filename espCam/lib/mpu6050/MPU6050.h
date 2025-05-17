#pragma once
#include <Wire.h>

class MPU6050
{
private:
    uint8_t address;
    int16_t AcX, AcY, AcZ;
    int16_t GyX, GyY, GyZ;
    int16_t Temp;

    // Scale factors
    float accelScale;
    float gyroScale;

public:
    MPU6050(uint8_t addr = 0x68);
    void begin(int sdaPin = 8, int sclPin = 9, uint32_t clock = 400000);
    bool update();

    // RAW getters
    int16_t getAccXRaw() const;
    int16_t getAccYRaw() const;
    int16_t getAccZRaw() const;
    int16_t getGyroXRaw() const;
    int16_t getGyroYRaw() const;
    int16_t getGyroZRaw() const;

    // Converted getters
    float getAccXg() const;
    float getAccYg() const;
    float getAccZg() const;
    float getGyroXdeg() const;
    float getGyroYdeg() const;
    float getGyroZdeg() const;

    // High-level data
    float getTemperature() const;
    float getPitch() const;
    float getRoll() const;
    float getAccMagnitude() const;
};
