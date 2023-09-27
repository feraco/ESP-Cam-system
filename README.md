ESP32 Timelapse System 📷

A timelapse capture system using an ESP32 and the AI Thinker camera module. This project is designed to assist in science investigations, allowing for time-stretched recording to observe slow changes.

Features

Timelapse Capturing: Take pictures every 2 minutes (configurable).
Web Server Interface: Access captured images via a web interface served directly from the ESP32.
SD Card Storage: Store images to an SD card, making it easy to retrieve and view them later.
Getting Started

Prerequisites
Hardware:
ESP32 board
AI Thinker Camera module
SD Card
Software:
Arduino IDE with ESP32 support
Required libraries:
ESPAsyncWebServer
ESPAsyncWebSrv
AsyncTCP
esp_camera
SD_MMC
Installation
Clone this repository:
bash
Copy code
git clone <[your-repo-link](https://github.com/feraco/ESP-Cam-system/)>
Open the project in the Arduino IDE.
Install the required libraries via the Library Manager.
Connect the camera module and SD card to your ESP32 as per the defined pins in the code.
Upload the code to your ESP32.
Usage
Once the ESP32 is powered on, it creates its own WiFi access point. Connect to the ESP32-Access-Point using the password 123456789.
Open a browser and navigate to http://192.168.4.1/ (or the IP address displayed in the serial monitor).
You will see a list of captured images. Click on any image to view it.
To take pictures, ensure the system is powered on. Pictures are taken automatically at the defined interval.
Contribution

Pull requests are welcome! For major changes, please open an issue first to discuss the proposed change.
