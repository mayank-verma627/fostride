#include <ESP32Servo.h>

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// VL53L0X pins
#define SDA_PIN 21
#define SCL_PIN 22
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const float THRESHOLD_CM = 25.0;

// Serial pins
#define ESP32_RX2 16
#define ESP32_TX2 17

// Servo pins
#define SERVO_BASE_PIN 12
#define SERVO_TILT_PIN 14

Servo servoBase;
Servo servoTilt;

// Metal sensor pin
#define METAL_SENSOR_PIN 15

// Servo angles
const int BASE_BIN1 = 45;
const int BASE_BIN2 = 135;
const int BASE_BIN3 = 45;  // bin3 doesn't rotate base
const int BASE_BIN4 = 135;

const int TILT_HOME = 92;
const int TILT_FORWARD = 60;
const int TILT_BACKWARD = 150;

// Smooth motion
const int STEP_SIZE = 2;
const int STEP_DELAY = 0;

// Track current base position
int currentBaseAngle = BASE_BIN1;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, ESP32_RX2, ESP32_TX2);

  Wire.begin(SDA_PIN, SCL_PIN); 
  if (!lox.begin()) while(1);

  servoBase.attach(SERVO_BASE_PIN);
  servoTilt.attach(SERVO_TILT_PIN);

  servoBase.write(currentBaseAngle);
  servoTilt.write(TILT_HOME);

  pinMode(METAL_SENSOR_PIN, INPUT);

  Serial.println("Bin Ready");
}

void loop() {
  // Metal sensor override
  if (digitalRead(METAL_SENSOR_PIN) == HIGH) {
    Serial.println("Metal detected -> Bin 3");
    moveToBin(3);
    delay(500); 
    return;
  }

  // Lidar check
  bool objectDetected = lidar();
  if (objectDetected) {
    Serial.println("Object detected");
    Serial2.println(true);

    while (Serial2.available() == 0); // wait for serial

    int received = Serial2.parseInt();
    int bin = convert(received);
    Serial.print("Received bin: ");
    Serial.println(bin);

    moveToBin(bin);
  } else {
    Serial2.println(false);
  }
}

// Move to bin intelligently
void moveToBin(int bin) {
  int targetBase = currentBaseAngle;
  int targetTilt = TILT_HOME; // Tilt always starts from home

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

  // Move base if needed
  if (currentBaseAngle != targetBase) moveServoSmooth(servoBase, currentBaseAngle, targetBase);
  delay(500);
  // Move tilt to target
  moveServoSmooth(servoTilt, TILT_HOME, targetTilt);
  delay(500);
  // Return tilt to home every time
  moveServoSmooth(servoTilt, targetTilt, TILT_HOME);

  // Update current base position
  currentBaseAngle = targetBase;
}

// Smooth servo motion
void moveServoSmooth(Servo &servo, int startAngle, int endAngle) {
  if (startAngle < endAngle) {
    for (int a = startAngle; a <= endAngle; a += STEP_SIZE) {
      servo.write(endAngle);
      delay(STEP_DELAY);
    }
  } else {
    for (int a = startAngle; a >= endAngle; a -= STEP_SIZE) {
      servo.write(endAngle);
      delay(STEP_DELAY);
    }
  }
}

// Convert serial input
int convert(int a) { return (a > 10) ? a % 10 : a; }

// Lidar read
bool lidar() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) Serial.println(measure.RangeMilliMeter);
  return measure.RangeMilliMeter < 300;
}
