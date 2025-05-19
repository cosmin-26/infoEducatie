#include <Arduino.h>
#include "Wireless.h"
#include "ESP32Cam.h"


// Create camera server on port 80
ESP32CamServer cam(80);

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wireless::Wifi::getInstance("Info2","parola#info")->connectWifi();
    cam.begin();


}



void loop() {
  delay(100);
}
