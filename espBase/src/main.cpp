#include<Arduino.h>
#include <WiFi.h>
#include "Wireless.h"
#include "DTH11.h"

DTH11 dth(33);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 INITIALISED");
    Wireless::Wifi::getInstance()->connectWifi();
    dth.begin();
}

void loop() {
    Wireless::Wifi::getInstance()->printWifiStatus();
    dth.sendData();
    time_t temp = time(0);
    Serial.println(ctime(&temp));
    Serial.println(dth.toString());
    delay(600000);
}
