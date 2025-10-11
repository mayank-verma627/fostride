#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "esp32";
const char* password = "mayank627";

// Laptop IP and port number (same as in the Python server)
const char* serverIP = "192.168.247.83";  // Replace with your laptop's IP
const uint16_t serverPort = 1234;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // Try to connect to TCP server
  Serial.print("Connecting to server ");
  Serial.println(serverIP);
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to server!");
  } else {
    Serial.println("Connection failed.");
  }
}

void loop() {
  if (client.connected()) {
    String message = "Hello from ESP32!";
    client.println(message); // Send message to laptop
    Serial.println("Sent: " + message);
  } else {
    Serial.println("Disconnected. Trying to reconnect...");
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Reconnected to server.");
    } else {
      Serial.println("Reconnect failed.");
    }
  }

  delay(1000); // Wait 1 second
}
