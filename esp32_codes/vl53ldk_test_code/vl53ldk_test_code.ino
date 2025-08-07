#include <ESP32_OTA_Handler.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>


#define SDA_PIN 21
#define SCL_PIN 22

const char* ssid         = "esp32";
const char* password     = "mayank627";
const char* hostname     = "fostrideESP32_model_1";

ESP32_OTA_Handler otaHandler(ssid, password, hostname);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X. Check wiring.");
    while (1);
  }
  Serial.println("VL53L0X ready");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Out of range");
  }

  delay(200);
}
