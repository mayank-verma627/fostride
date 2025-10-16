#include <ESP32Servo.h>
#include "BluetoothSerial.h"  // Built-in library

BluetoothSerial SerialBT;

// Servo pins
#define SERVO_BASE_PIN 12
#define SERVO_TILT_PIN 14

Servo servoBase;
Servo servoTilt;

const int BASE_BIN1 = 45;
const int BASE_BIN2 = 135;
const int BASE_BIN3 = 45;  // bin3 doesn't rotate base
const int BASE_BIN4 = 135;

const int TILT_HOME = 92;
const int TILT_FORWARD = 70;
const int TILT_BACKWARD = 140;

// Smooth motion
const int STEP_SIZE = 2;
const int STEP_DELAY = 0;

// Track current base position
int currentBaseAngle = BASE_BIN1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT");  // Bluetooth device name
  
  Serial.println("Bluetooth started! Pair your phone with ESP32_BT.");
  servoBase.attach(SERVO_BASE_PIN);
  servoTilt.attach(SERVO_TILT_PIN);

  servoBase.write(currentBaseAngle);
  servoTilt.write(TILT_HOME);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (SerialBT.available()) {
    String incoming = SerialBT.readStringUntil('\n'); // Read until Enter
    Serial.print("Received via BT: ");
    Serial.println(incoming);
    moveToBin(incoming.toInt());
    delay(500);
  }

}


void moveToBin(int bin) {
  int targetBase = currentBaseAngle;
  int targetTilt = TILT_HOME;

  switch(bin) {
    case 1:
      targetBase = BASE_BIN1;
      targetTilt = TILT_FORWARD;
      break;
    case 2:
      targetBase = BASE_BIN2;
      targetTilt = TILT_FORWARD;
      break;
    case 3:
      targetBase = BASE_BIN3;
      targetTilt = TILT_BACKWARD;
      break;
    case 4:
      targetBase = BASE_BIN4;
      targetTilt = TILT_BACKWARD;
      break;
    default:
      Serial.println("Invalid bin");
      return;
  }

  if (currentBaseAngle != targetBase) moveServoSmooth(servoBase, currentBaseAngle, targetBase);
  delay(500);
  moveServoSmooth(servoTilt, TILT_HOME, targetTilt);
  delay(500);
  moveServoSmooth(servoTilt, targetTilt, TILT_HOME);

  currentBaseAngle = targetBase;
}

// Smooth servo motion
void moveServoSmooth(Servo &servo, int startAngle, int endAngle) {
  if (startAngle < endAngle) {
    for (int a = startAngle; a <= endAngle; a += STEP_SIZE) {
      servo.write(a);
      delay(STEP_DELAY);
    }
  } else {
    for (int a = startAngle; a >= endAngle; a -= STEP_SIZE) {
      servo.write(a);
      delay(STEP_DELAY);
    }
  }
}
