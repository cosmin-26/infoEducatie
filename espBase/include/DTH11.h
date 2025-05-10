#pragma once
#include <Arduino.h>
#include <DHT.h>
#include "Sensor.h"

class DTH11: public Sensor {
private:
    uint8_t _pin;
    DHT _dht;
    SensorData _values[2];
    
public:
    DTH11(uint8_t pin);
    virtual ~DTH11();

    void begin();
    SensorData* getReading() override;
    String toString() override;
    void sendData() override;
};
