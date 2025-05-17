#include <Arduino.h>

#define PIN_FLASHLIGHT 4

void setup() {
  delay(2000);  // Allow time for stable boot
  pinMode(PIN_FLASHLIGHT, OUTPUT);
}

void loop() {
  digitalWrite(PIN_FLASHLIGHT, LOW);  // Turn ON
  delay(700);
  digitalWrite(PIN_FLASHLIGHT, HIGH); // Turn OFF
  delay(700);
}
