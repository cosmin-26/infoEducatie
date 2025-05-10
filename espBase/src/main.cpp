#include<Arduino.h>
#include <WiFi.h>
#include "secrets.h"

const char* ssid = HOME_SSID;
const char* password = HOME_PASSWORD;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting WiFi Test...");

  WiFi.disconnect(true); // Reset WiFi
  delay(100);

  WiFi.mode(WIFI_STA); // Required
  delay(100);

  Serial.printf("Status BEFORE begin: %d\n", WiFi.status());

  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.printf("Status AFTER connect: %d\n", WiFi.status());

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi FAILED");
  }
}

void loop() {}
