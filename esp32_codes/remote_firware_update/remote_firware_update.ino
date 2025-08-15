#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

#define WIFI_SSID      "Esp32"
#define WIFI_PASSWORD  "mayank@627"

// GitHub raw links
#define VERSION_URL "https://raw.githubusercontent.com/mayank-verma627/fostride/refs/heads/master/esp32_codes/remote_firware_update/version.txt"
#define FIRMWARE_URL "https://raw.githubusercontent.com/mayank-verma627/fostride/master/esp32_codes/remote_firware_update/remote_firware_update.ino.esp32.bin"

#define led 23
#define val 100

String currentVersion = "1.0.5";  // Version in current firmware

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  
  if (checkForUpdate()) {
    Serial.println("New version found! Starting OTA...");
    if (performOTA()) {
      Serial.println("Update successful! Rebooting...");
      ESP.restart();
    } else {
      Serial.println("Update failed!");
    }
  } else {
    Serial.println("No update needed.");
  }

  pinMode(led, OUTPUT);
}

void loop() {
  // Normal operation code here
  digitalWrite(led, HIGH);
  delay(val);
  digitalWrite(led, LOW);
  delay(val);
  
  if (checkForUpdate()) {
    Serial.println("New version found! Starting OTA...");
    if (performOTA()) {
      Serial.println("Update successful! Rebooting...");
      ESP.restart();
    } else {
      Serial.println("Update failed!");
    }
  } else {
    Serial.println("No update needed.");
  }
 
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

bool checkForUpdate() {
  HTTPClient http;
  http.begin(VERSION_URL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String newVersion = http.getString();
    newVersion.trim();
    Serial.println("Latest version: " + newVersion);
    if (newVersion != currentVersion) {
      return true;
    }
  } else {
    Serial.println("Version check failed, HTTP code: " + String(httpCode));
  }
  http.end();
  return false;
}

bool performOTA() {
  HTTPClient http;
  http.begin(FIRMWARE_URL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    int contentLength = http.getSize();
    WiFiClient *client = http.getStreamPtr();

    if (contentLength > 0) {
      Serial.println("Content-Length: " + String(contentLength));
      if (!Update.begin(contentLength)) {
        Serial.println("Not enough space for OTA");
        return false;
      }
    } else {
      Serial.println("No Content-Length header, starting OTA with unknown size");
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Serial.println("Not enough space for OTA");
        return false;
      }
    }

    size_t written = Update.writeStream(*client);
    if (written > 0) {
      Serial.println("OTA written: " + String(written));
    } else {
      Serial.println("OTA write failed");
      return false;
    }

    if (Update.end()) {
      if (Update.isFinished()) {
        Serial.println("Update successful");
        return true;
      } else {
        Serial.println("Update not finished");
        return false;
      }
    } else {
      Serial.println("Update error: " + String(Update.getError()));
      return false;
    }
  } else {
    Serial.println("Firmware download failed, HTTP code: " + String(httpCode));
    return false;
  }
  http.end();
  return false;
}
