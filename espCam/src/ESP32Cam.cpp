#include "ESP32Cam.h"

// Pin configuration for AI-Thinker ESP32-CAM
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22


void ESP32CamServer::begin() {
    setupCamera();
    startServer();
}

void ESP32CamServer::setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if(psramFound()){
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_CIF;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
    }
}

void ESP32CamServer::startServer() {
    server.on("/capture", HTTP_GET, [this](AsyncWebServerRequest *request){
        handleCapture(request);
    });

    server.on("/stream", HTTP_GET, [this](AsyncWebServerRequest *request){
        handleStream(request);
    });

    server.begin();
}

void ESP32CamServer::handleCapture(AsyncWebServerRequest *request) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        request->send(500, "text/plain", "Camera capture failed");
        return;
    }

    AsyncWebServerResponse * response = request->beginResponse(
        "image/jpeg", fb->len, [fb](uint8_t *buffer, size_t maxLen, size_t alreadySent) -> size_t {
            size_t toSend = fb->len - alreadySent;
            if (toSend > maxLen) toSend = maxLen;
            memcpy(buffer, fb->buf + alreadySent, toSend);
            if (alreadySent + toSend == fb->len) {
                esp_camera_fb_return(fb); // free the frame after sending
            }
            return toSend;
        }
    );

    response->addHeader("Content-Disposition", "inline; filename=capture.jpg");
    request->send(response);
}

//TODO:i need to make this better
void ESP32CamServer::handleStream(AsyncWebServerRequest *request) {
    /*
    WiFiClient client = request->client();
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    client.print(response);

    while (client.connected()) {
        camera_fb_t * fb = esp_camera_fb_get();
        if (!fb) break;

        client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", fb->len);
        client.write(fb->buf, fb->len);
        client.print("\r\n");
        esp_camera_fb_return(fb);
        delay(100); // adjust frame rate
    }
        */

}