#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your WiFi credentials
const char* ssid = "esp32";
const char* password = "mayank@627";

// Replace with your Apps Script Web App URL
String serverName = "https://script.google.com/a/macros/somaiya.edu/s/AKfycbyfzjSW6vQYzBlsq1FoIO6fgTu7hZ4FNTMv0HXm35lkeo0b-NjIquEYYh5K3LJ291QG/exec";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Start connection
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Example sensor values (replace with real sensor reads)
    float temperature = 25.4;
    float humidity = 61.2;

    // Create JSON payload
    String jsonData = "{\"temperature\":";
    jsonData += String(temperature, 2);
    jsonData += ", \"humidity\":";
    jsonData += String(humidity, 2);
    jsonData += "}";

    // Send POST request
    int httpResponseCode = http.POST(jsonData);

    // Print response
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("⚠️ WiFi Disconnected");
  }

  delay(10000); // send every 10 seconds
}
