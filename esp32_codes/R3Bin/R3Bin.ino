

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

#define WIFI_SSID      "Esp32"
#define WIFI_PASSWORD  "mayank@627"

// GitHub raw links
#define VERSION_URL "https://raw.githubusercontent.com/mayank-verma627/fostride/refs/heads/master/esp32_codes/R3Bin/version_R3Bin.txt"
#define FIRMWARE_URL "https://raw.githubusercontent.com/mayank-verma627/fostride/master/esp32_codes/R3Bin/R3Bin.ino.esp32.bin"

String currentVersion = "1.1.0";  // Version in current firmware

#include <AccelStepper.h>
#include <MultiStepper.h>

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

///////////i2c with lidar/////////////////
#define SDA_PIN 21
#define SCL_PIN 22
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const float THRESHOLD_CM = 25.0;
///////////////////////////////////////////

/////uart 2 for r pi///////////////////////
#define ESP32_RX2 16
#define ESP32_TX2 17
///////////////////////////////////////////

/////metallic sensor pinouts//////////////////
#define metal 4
////////////////////////////////////////////////

/////////stepper motor driver//////////////
#define ena 26
#define enb 33

#define PUL_PIN 25
#define DIR_PIN 27

#define PUL_PIN2 35 
#define DIR_PIN2 32

AccelStepper motor1(AccelStepper::DRIVER, PUL_PIN, DIR_PIN);
AccelStepper motor2(AccelStepper::DRIVER, PUL_PIN2, DIR_PIN2);
///////////////////////////////////////////

#define delay_ms 500  //delay between vertical tilt of the plate
#define delay_ms2 1500  //delay between the rotationof the plate after the complete motion

int currentLocation=1;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  connectToWiFi();

  RemoteUpdate();
  
  Serial.println("Welcome to R3Bin");
  Serial.println("Version: ");
  Serial.print(currentVersion);
  
  delay(5000);
  Serial.println("Bin Setting Up....Please Wait");
  delay(1000);
  Serial2.begin(115200, SERIAL_8N1, ESP32_RX2, ESP32_TX2);

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  digitalWrite(ena , HIGH);
  digitalWrite(enb, HIGH);

  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(PUL_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

  pinMode(metal, INPUT);
  
  
  Wire.begin(SDA_PIN, SCL_PIN);

  motor1.setMaxSpeed(6000); //for slow speed
  motor1.setAcceleration(3000);

  motor2.setMaxSpeed(6000);
  motor2.setAcceleration(3000);

  Serial.println("Bin Ready To Use");

}

void loop() {
  // put your main code here, to run repeatedly:
  test();
  RemoteUpdate();
}

//////////to sense the incoming of the waste using lidar at a threshold/////////
bool lidar(){
  bool flag;
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Out of range");
  }
  if(measure.RangeMilliMeter<200 && measure.RangeMilliMeter!=0 ){
    flag=true;
  }
  else{
    flag=false;
  }

  return flag;
}
//////////////////////////////////////////////////////////////////////////


//////////to filter the incoming data from the r pi////////////
int convert(int a){
  int val;
  if(a>10){
    val=a%10;
  }
  else{
    val=a;
  }
  return val;
}
/////////////////////////////////////////////////////////////////

///////////moving the plates to different bins////////////////////
void differential_control(int value){
  if(value==1){
    Serial.println("Bin 1");
  moveMotor1(150);
  delay(delay_ms);

  moveMotor1(-150);
  delay(delay_ms2);
  }

  else if(value==2){
        Serial.println("Bin 2");
  moveBoth(300);
  delay(delay_ms);

  moveMotor1(150);
  delay(delay_ms);

  moveMotor1(-150);
  delay(delay_ms);

  moveBoth(-300);
  delay(delay_ms2);
  }

  else if(value==3){
        Serial.println("Bin 3");
  moveMotor1(-150);
  delay(delay_ms);

  moveMotor1(150);
  delay(delay_ms2);
  }

  else if(value==4) {
        Serial.println("Bin 4");
  moveBoth(300);
  delay(delay_ms);

  moveMotor1(-150);
  delay(delay_ms);

  moveMotor1(150);
  delay(delay_ms);

  moveBoth(-300);
  delay(delay_ms2);
  }
  else{
    
  }
}

void moveMotor1(int steps) {
  if(steps<0){digitalWrite(DIR_PIN, HIGH); steps=steps*-1;}
  else{digitalWrite(DIR_PIN, LOW);}
  for(int i=0;i<steps;i++){
  digitalWrite(PUL_PIN, HIGH);
  delayMicroseconds(200);
  digitalWrite(PUL_PIN, LOW);
  delayMicroseconds(200);
  }
}

void moveMotor2(int steps) {
  if(steps<0){digitalWrite(DIR_PIN2, HIGH); steps=steps*-1;}
  else{digitalWrite(DIR_PIN2, LOW);}
  for(int i=0;i<steps;i++){
  digitalWrite(PUL_PIN2, HIGH);
  delayMicroseconds(200);
  digitalWrite(PUL_PIN2, LOW);
  delayMicroseconds(200);
  }
}

void moveBoth(int steps) {
  if(steps<0){
    digitalWrite(DIR_PIN, HIGH); 
    digitalWrite(DIR_PIN2,LOW);
    steps=steps*-1;
    }
  else{
    digitalWrite(DIR_PIN, LOW);
    digitalWrite(DIR_PIN2, HIGH);
    }
  for(int i=0;i<steps;i++){
  digitalWrite(PUL_PIN, HIGH);
  digitalWrite(PUL_PIN2, HIGH);
  delayMicroseconds(200);
  digitalWrite(PUL_PIN, LOW);
  digitalWrite(PUL_PIN2, LOW);
  delayMicroseconds(200);
  }
}
//////////////////////////////////////////////////////////////////////////

//////to test the working of the motors//////////////////////////////
void test(){
  digitalWrite(ena,LOW);
  digitalWrite(enb, LOW);
  differential_control(1);
  delay(2000);
  differential_control(2);
  delay(2000);
  differential_control(3);
  delay(2000);
  differential_control(4);
  delay(2000);
}
///////////////////////////////////////////////////////////////////////










void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void RemoteUpdate(){
  
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




bool checkForUpdate() {
  HTTPClient http;
  http.begin(VERSION_URL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String newVersion = http.getString();
    newVersion.trim();

    Serial.println("Local version: " + currentVersion);
    Serial.println("Latest version on server: " + newVersion);

    if (newVersion != currentVersion) {
      Serial.println("Update required!");
      return true;
    } else {
      Serial.println("Already on latest version.");
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
