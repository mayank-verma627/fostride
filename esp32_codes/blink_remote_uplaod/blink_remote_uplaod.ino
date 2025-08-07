#include <ESP32_OTA_Handler.h>


const char* ssid         = "esp32";
const char* password     = "mayank627";
const char* hostname     = "fostrideESP32_model_1";
const char* ota_password = "fostride@627";

ESP32_OTA_Handler otaHandler(ssid, password, hostname);

#define LED_BUILTIN 5

void setup() {
  pinMode(5, OUTPUT);
  otaHandler.begin();
}

void loop() {
  digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                     // wait for a second
  otaHandler.handle();
}
