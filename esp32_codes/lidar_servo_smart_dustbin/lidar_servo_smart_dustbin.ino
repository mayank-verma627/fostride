#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  // Default I2C address 0x40

#define SERVO_CHANNEL 0
#define SERVO_MIN 150   // Min pulse length (0 degrees)
#define SERVO_MAX 600   // Max pulse length (180 degrees)

bool objectDetected = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // Explicitly set ESP32 SDA, SCL pins

  // Start VL53L0X sensor
  if (!lox.begin()) {
    Serial.println("Failed to detect VL53L0X. Check wiring!");
    while (1);
  }

  // Start PWM driver (HW-170)
  pwm.begin();
  pwm.setPWMFreq(50);  // 50 Hz for servos

  moveServoTo(0);  // Start at 0 degrees
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {  // 4 = out of range
    int distance = measure.RangeMilliMeter;
    Serial.print("Distance (mm): "); Serial.println(distance);

    if (distance < 250 && !objectDetected) {
      objectDetected = true;
      moveServoTo(180);
    } else if (distance >= 250 && objectDetected) {
      objectDetected = false;
      moveServoTo(0);
    }
  } else {
    Serial.println("Out of range");
  }

  delay(100);
}

void moveServoTo(int angle) {
  int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(SERVO_CHANNEL, 0, pulse);
  Serial.print("Moved to "); Serial.print(angle); Serial.println(" degrees");
}
