#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include "Wireless.h"

struct SensorData {
    const char* key;
    float value;
    const char* unit;
};

class Sensor{
protected:
    uint8_t _count;
    SensorData* _data;
    String _serverUrl = "http://192.168.0.124:5081/esp-data";
    HTTPClient _http;
    String _sensorType="default",_room="main lab";

public:
    Sensor();
    virtual ~Sensor();

    virtual SensorData* getReading();
    virtual String toJson();
    virtual String toString();
    virtual void sendData();

    void setUrl(const char* url);
    void setRoom(const char* room);

};



