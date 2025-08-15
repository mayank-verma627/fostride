#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

#define WIFI_SSID      "Esp32"
#define WIFI_PASSWORD  "mayank@627"

// GitHub raw links
#define VERSION_URL "https://raw.githubusercontent.com/mayank-verma627/fostride/refs/heads/master/esp32_codes/remote_firware_update/version.txt"
#define FIRMWARE_URL "https://github.com/mayank-verma627/fostride/blob/master/esp32_codes/remote_firware_update/remote_firware_update.ino.esp32.bin"

String currentVersion = "1.0.2";  // Version in current firmware

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
}

void loop() {
  // Normal operation code here
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
      if (!Update.begin(contentLength)) {
        Serial.println("Not enough space for OTA");
        return false;
      }
      size_t written = Update.writeStream(*client);
      if (written == contentLength) {
        Serial.println("OTA written: " + String(written));
      } else {
        Serial.println("OTA write mismatch: " + String(written) + " / " + String(contentLength));
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
      Serial.println("Invalid content length");
      return false;
    }
  } else {
    Serial.println("Firmware download failed, HTTP code: " + String(httpCode));
    return false;
  }
  http.end();
  return false;
}
