#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ================= Wi-Fi =================
const char* ssid = "Esp32";
const char* password = "mayank@627";

// ================= MQTT =================
const char* mqtt_server = "7c3fb6661d89423d9c0bb8e473148d6b.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "R3Bin";
const char* mqtt_password = "R3Bin_fostride@627";
const char* publish_topic = "R3Bin/data";

// ================= TLS Root CA =================
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
...
-----END CERTIFICATE-----
)EOF";

// ================= Bin Info =================
const String location = "Mumbai_ridll";
const int bin_number = 1;
const String deployDate = "20251015";
String binId = "R3Bin_" + location + "_" + String(bin_number) + "_" + deployDate;

// ================= MQTT Client =================
WiFiClientSecure espClient;
PubSubClient client(espClient);

// ================= Timer =================
unsigned long lastSend = 0;
const long sendInterval = 10000; // 10 seconds

// ================= Functions =================
void reconnect(){
  while(!client.connected()){
    Serial.print("Connecting to MQTT Broker...");
    if(client.connect(binId.c_str(), mqtt_username, mqtt_password)){
      Serial.println("Connected!");
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void sendBinData(){
  StaticJsonDocument<256> doc; // increased size for extra fields

  doc["bin_id"] = binId;
  String jsonString;
  serializeJson(doc, jsonString);
  client.publish(publish_topic, jsonString.c_str());
  Serial.println("Published: " + jsonString);
}

// ================= Setup =================
void setup(){
  Serial.begin(115200);
  Serial.println("R3Bin ESP32 starting...");

  // Wi-Fi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());

  // TLS setup
  espClient.setCACert(root_ca);

  // MQTT setup
  client.setServer(mqtt_server, mqtt_port);
}

// ================= Loop =================
void loop(){
  if(!client.connected()) reconnect();
  client.loop();

  // Publish every 10 seconds (example: random test data)
  if(millis() - lastSend > sendInterval){
    lastSend = millis();
    sendBinData();
  }
}
