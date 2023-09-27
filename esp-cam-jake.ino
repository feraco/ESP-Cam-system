// Including necessary libraries for the timelapse system
#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>

#include <WiFi.h>              // Library for WiFi connectivity of ESP32
#include <AsyncTCP.h>
#include <esp_camera.h>        // Library for camera functionalities
#include <FS.h>                // File System library for ESP32
#include <SD_MMC.h>            // Library for SD card functionalities

// Pin definitions for the AI Thinker model camera module
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
...  // Continuing with other pin definitions

// Network credentials for setting up ESP32 Access Point
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

#define CAMERA_MODEL_AI_THINKER
AsyncWebServer server(80);  // Setting up an asynchronous web server on port 80

void setup() {
   // Initializing serial communication
   Serial.begin(115200); 

   // Setting up the ESP32 as an Access Point
   WiFi.softAP(ssid, password);
   IPAddress IP = WiFi.softAPIP();
   Serial.print("AP IP address: ");
   Serial.println(IP);

   // Defining route to fetch images from the SD card
   server.on("/get/", HTTP_GET, [](AsyncWebServerRequest *request) {
     String path = request->url().substring(4);
     File file = SD_MMC.open(path, FILE_READ);
     if (!file) {
       request->send(404, "text/plain", "File not found");
       return;
     }
     request->send(SD_MMC, path, "image/jpeg");
     file.close();
   });

   server.begin();  // Starting the server

   // Setting up and initializing the camera
   camera_config_t config;
   ...  // Configuration settings for the camera

   // Initializing the camera and handling any errors
   esp_err_t err = esp_camera_init(&config);
   if (err != ESP_OK) {
     Serial.printf("Camera init failed with error 0x%x", err);
     return;
   }

   // Initializing and setting up the SD card
   if (!SD_MMC.begin()) {
     Serial.println("SD Card Mount Failed");
     return;
   }
   uint8_t cardType = SD_MMC.cardType();
   if (cardType == CARD_NONE) {
     Serial.println("No SD Card attached");
     return;
   }
   Serial.println("SD Card initialized.");
}

void loop() {
   // Routine to take a picture every 2 minutes
   static unsigned long lastPictureTime = 0;
   if (millis() - lastPictureTime > 120000) {
     takePicture();
     lastPictureTime = millis();
   }
   delay(1000);  // Pausing the loop for 1 second
}

void handleRoot(AsyncWebServerRequest *request) {
   // Serve a list of images on visiting the root directory
   String html = "<html><body>";
   File root = SD_MMC.open("/");
   File file = root.openNextFile();
   while (file) {
     html += "<a href='/get" + String(file.name()) + "'>" + String(file.name()) + "</a><br>";
     file.close();
     file = root.openNextFile();
   }
   html += "</body></html>";
   request->send(200, "text/html", html);
}

void takePicture() {
   // Function to capture and save the image to SD card
   Serial.println("Taking picture...");
   camera_fb_t * fb = esp_camera_fb_get();
   if (!fb) {
     Serial.println("Camera capture failed");
     return;
   }
   char path[20];
   sprintf(path, "/pic%d.jpg", millis());
   File file = SD_MMC.open(path, FILE_WRITE);
   if (!file) {
     Serial.println("Failed to open file for writing");
     return;
   }
   file.write(fb->buf, fb->len);
   Serial.printf("Saved to %s\n", path);
   file.close();
   esp_camera_fb_return(fb);
}
