#include <AccelStepper.h>
#include <MultiStepper.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

////////////pins for i2c with lidar sensor//////////////////////
#define SDA_PIN 21
#define SCL_PIN 22
#define address_bin1
#define address_bin2
#define address_bin3
#define address_bin4
#define address_gate
////////////////////////////////////////////////////////////////

////////////////////lidar objects and variables/////////////////
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const float THRESHOLD_CM = 25.0;
////////////////////////////////////////////////////////////////

///////////uart pins for r pi///////////////////////////////////
#define ESP32_RX2 16
#define ESP32_TX2 17
///////////////////////////////////////////////////////////////

///////////stepper motor driver control pins///////////////////
#define PUL_PIN 15
#define DIR_PIN 2

#define PUL_PIN2 23 
#define DIR_PIN2 19
AccelStepper motor1(AccelStepper::DRIVER, PUL_PIN, DIR_PIN);
AccelStepper motor2(AccelStepper::DRIVER, PUL_PIN2, DIR_PIN2);
///////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
