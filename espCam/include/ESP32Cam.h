#pragma once

#include <esp_camera.h>
#include <ESPAsyncWebServer.h>

class ESP32CamServer {
public:
    ESP32CamServer(int port = 80);
    void begin();
    void setupCamera();
    void startServer();

private:
    AsyncWebServer server;
    void handleStream(AsyncWebServerRequest *request);
    void handleCapture(AsyncWebServerRequest *request);
};


