#pragma once 

#include <WebServer.h>
#include "esp_camera.h"

class ESP32CamServer {
public:
    ESP32CamServer(int port = 80);

    void begin();
    void handleClient();

private:
    WebServer server;
    int _port;

    void setupCamera();
    void setupRoutes();
    
    static void handleCapture();
    static void handleStream();
};
