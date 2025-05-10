#include "MPU6050.h"
#include <math.h> // pentru sqrt() și atan2()

MPU6050::MPU6050(uint8_t addr) : address(addr), accelScale(16384.0), gyroScale(131.0) {}

void MPU6050::begin(int sdaPin, int sclPin, uint32_t clock)
{
    Wire.begin(sdaPin, sclPin, clock);

    Wire.beginTransmission(address);
    Wire.write(0x6B); // Power management 1
    Wire.write(0);    // Wake up
    Wire.endTransmission(true);
}

bool MPU6050::update()
{
    Wire.beginTransmission(address);
    Wire.write(0x3B);
    if (Wire.endTransmission(false) != 0)
    {
        return false;
    }
    Wire.requestFrom(address, (uint8_t)14);
    if (Wire.available() < 14)
    {
        return false;
    }

    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    Temp = Wire.read() << 8 | Wire.read();
    GyX = Wire.read() << 8 | Wire.read();
    GyY = Wire.read() << 8 | Wire.read();
    GyZ = Wire.read() << 8 | Wire.read();

    return true;
}

// RAW getters
int16_t MPU6050::getAccXRaw() const { return AcX; }
int16_t MPU6050::getAccYRaw() const { return AcY; }
int16_t MPU6050::getAccZRaw() const { return AcZ; }
int16_t MPU6050::getGyroXRaw() const { return GyX; }
int16_t MPU6050::getGyroYRaw() const { return GyY; }
int16_t MPU6050::getGyroZRaw() const { return GyZ; }

// Converted getters
float MPU6050::getAccXg() const { return AcX / accelScale; }
float MPU6050::getAccYg() const { return AcY / accelScale; }
float MPU6050::getAccZg() const { return AcZ / accelScale; }

float MPU6050::getGyroXdeg() const { return GyX / gyroScale; }
float MPU6050::getGyroYdeg() const { return GyY / gyroScale; }
float MPU6050::getGyroZdeg() const { return GyZ / gyroScale; }

// High-level
float MPU6050::getTemperature() const
{
    return (Temp / 340.0) + 36.53;
}

float MPU6050::getPitch() const
{
    return atan2(getAccYg(), sqrt(getAccXg() * getAccXg() + getAccZg() * getAccZg())) * 180.0 / M_PI;
}

float MPU6050::getRoll() const
{
    return atan2(-getAccXg(), getAccZg()) * 180.0 / M_PI;
}

float MPU6050::getAccMagnitude() const
{
    return sqrt(getAccXg() * getAccXg() + getAccYg() * getAccYg() + getAccZg() * getAccZg());
}
