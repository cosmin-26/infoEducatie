#include "Wireless.h"
#include "secrets.h"
using namespace Wireless;

Wifi *Wifi::_instance = nullptr;

// Default constructor (uses predefined SSID and Password)
Wifi::Wifi() : _ssid(HOME_SSID), _password(HOME_PASSWORD) {
    
}

// Constructor with custom SSID and Password
Wifi::Wifi(const char *ssid, const char *password) : _ssid(ssid), _password(password)
{
    Serial.print("Initialized");

}

// Singleton method: returns the single instance of Wifi (with custom credentials)
Wifi *Wifi::getInstance(const char *ssid, const char *password)
{
    if (_instance == nullptr)
    {
        _instance = new Wifi(ssid, password); 

    }
    return _instance;
}

Wifi *Wifi::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new Wifi();
    }
    return _instance;
}

Wifi::~Wifi()
{

}
void Wifi::connectWifi()
{
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(_ssid ? _ssid : "nullptr");

    Serial.println(1);
    if (!_ssid || strlen(_ssid) == 0)
    {
        Serial.println("ERROR: SSID is null or empty!");
        return;
    }

    Serial.printf("SSID: %s\n", _ssid ? _ssid : "NULL");
    Serial.printf("PASS: %s\n", _password ? _password : "NULL");

    WiFi.disconnect(true);         // Clear previous config

    WiFi.mode(WIFI_STA);           // Force Station mode

    delay(100);                    // Give it time

    WiFi.useStaticBuffers(true);
    WiFi.begin(_ssid, _password);

    const unsigned long timeout = 25000;
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout)
    {
        delay(300);
        Serial.print(".");
        delay(300);
    }



    if (WiFi.status() == WL_CONNECTED)
    {
        _ip = WiFi.localIP();
        Serial.println("Connected to WiFi");

        printWifiStatus();
    }
    else
    {
        Serial.println("ERROR: Failed to connect to WiFi.");
    }
}

bool Wifi::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress Wifi::getIP()
{
    return _ip;
}

void Wifi::printWifiStatus()
{
    Serial.print("SSID: ");
    Serial.println(_ssid);

    IPAddress ip = getIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
