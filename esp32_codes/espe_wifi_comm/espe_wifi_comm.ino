#include <WiFi.h>

// Change these to your phone hotspot name & password
const char* ssid = "MyPhoneHotspot";
const char* password = "12345678";

WiFiServer server(80); // TCP server on port 80

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to hotspot!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String incomingData = client.readStringUntil('\n'); // Read until newline
    if (incomingData.length() > 0) {
      Serial.print("Received: ");
      Serial.println(incomingData);
    }
  }
}
