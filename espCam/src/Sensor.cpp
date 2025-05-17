#include <Sensor.h>

Sensor::Sensor(){
    ;
}

Sensor::~Sensor(){
    ;
}

SensorData* Sensor::getReading(){
    return _data;
}

String Sensor::toJson() {
    String json = "{\"sensor_type\":\"" +String(_sensorType)+"\"";
    json+= ",\"data\":{";
    for (int i = 0; i < _count; i++) {
      json += "\"" + String(_data[i].key) + "\":{";
      json += "\"value\":" + String(_data[i].value, 1) + ",";
      json += "\"unit\":\"" + String(_data[i].unit) + "\"";
      json += "}";
      if (i < _count - 1) json += ",";
    }
    json += "},\"room\":\""+_room +"\"}";
    return json;
}

String Sensor::toString(){
    String result;
    for (int i = 0; i < _count; i++)
    {
        result +=  String(_data[i].value)+_data[i].unit+'\n';
    }
    return result;
}

void Sensor::setUrl(const char* url){
    _serverUrl = url;
}

void Sensor::sendData(){
    Serial.println("Sensor::sendData");
    if (Wireless::Wifi::getInstance()->isWiFiConnected()) {

        _http.begin(_serverUrl);
        _http.addHeader("Content-Type", "application/json");

        String payload = Sensor::toJson();
        
        Serial.println(payload);
        int httpResponseCode = _http.POST(payload);

        if (httpResponseCode > 0) {
            String response = _http.getString();
            Serial.println("Server response:");
            Serial.println(response);
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }

        _http.end();
    } else {
        Serial.println("WiFi not connected");
    }
}

void Sensor::setRoom(const char*room){
    _room = room;
}