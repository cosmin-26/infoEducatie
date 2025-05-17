#pragma once

#include <Arduino.h>
#include <WiFi.h>

namespace Wireless
{
    // namespace Wireless
    class Wifi
    {
    private:
        const char *_ssid, *_password;
        IPAddress _ip;


        Wifi();
        Wifi(const char *ssid, const char *password);

        static Wifi *_instance;
    public:
        ~Wifi();

        static Wifi*getInstance();
        static Wifi*getInstance(const char *ssid, const char *password);

        void connectWifi();
        IPAddress getIP();
        bool isWiFiConnected();

        void printWifiStatus();
    };
    class Ble
    {
    private:
        std::string _deviceName = "ESP32_BLE";

    public:
        Ble() {}
        Ble(const std::string &deviceName) : _deviceName(deviceName) {}

        void startBLE();

        void stopBLE();
    };
}