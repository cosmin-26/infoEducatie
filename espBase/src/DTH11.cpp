#include "DTH11.h"

DTH11::DTH11(uint8_t pin) : _pin(pin), _dht(pin, 11) {
    _count = 2;
    _values[0] = {"temperature",0,"C"};
    _values[1] = {"humidity",0,"%"};
    _data = _values;
}

DTH11::~DTH11() {}

void DTH11::begin() {
    _dht.begin();
}

SensorData* DTH11::getReading() {
    _data[0].value = _dht.readTemperature();
    _data[1].value = _dht.readHumidity();
    return _data;
}

String DTH11::toString(){
    this->getReading();
    return Sensor::toString();
}

void DTH11::sendData(){
    this->getReading();
    Sensor::sendData();
}

