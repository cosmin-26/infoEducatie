/*#include <Arduino.h>
#include <HardwareSerial.h>
#include <vector>
#include <LEDController.h>
#include <Wireless.h>
#include <Button.h>

uint8_t led[] = {0, 1};
LEDController wifiLED(led);
uint8_t bl = 3;
LEDController blue(&bl);
Wireless::Wifi wifi;
Button button(21);

void setup()
{

    Serial.begin(115200);
    while (!Serial)
    {
    }
    delay(2000);
    Serial.println("Connected to ESP32-C3");
    delay(100);
    wifi.setLED(&wifiLED);
    wifi.connectWifi();
    delay(100);
    wifi.printWifiStatus();
}

void loop()
{
    if (button.checkLongPress(700))
    {
        // wifi.connectWifi();
        ;
    }
    else if (button.checkToggle())
        Serial.println("HIELLOOO");

    if (button.check())
        blue.on();
    else
        blue.off();
}
*/