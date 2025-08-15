#include "BluetoothSerial.h"  // Built-in library

BluetoothSerial SerialBT;  // Create BT object

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT");  // Bluetooth device name
  Serial.println("Bluetooth started! Pair your phone with ESP32_BT.");
}

void loop() {
  if (SerialBT.available()) {
    String incoming = SerialBT.readStringUntil('\n'); // Read until Enter
    Serial.print("Received via BT: ");
    Serial.println(incoming);
  }
}
