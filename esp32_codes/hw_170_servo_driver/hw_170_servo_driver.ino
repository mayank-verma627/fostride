#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create the PWM driver object with default I2C address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Adjust these values based on your servo specs
#define SERVOMIN  150  // Minimum pulse length count (~0°)
#define SERVOMAX  600  // Maximum pulse length count (~180°)
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA = 21, SCL = 22 for ESP32
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  Serial.println("done");
}

void loop() {
  // Move servo on channel 0 from 0° to 180° and back
  for (int angle = 0; angle <= 180; angle += 10) {
    setServoAngle(0, 0);
    delay(30);
  }

  /*for (int angle = 180; angle >= 0; angle -= 10) {
    setServoAngle(0, angle);
    delay(30);
  }*/
}

// Converts angle (0 to 180) to PWM signal and sends to PCA9685
void setServoAngle(uint8_t channel, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(channel, 0, pulse);
}
