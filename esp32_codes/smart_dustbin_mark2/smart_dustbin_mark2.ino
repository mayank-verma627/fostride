#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create PWM driver instance with default I2C address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150  // Min pulse length
#define SERVOMAX  600  // Max pulse length
#define SERVO_FREQ 50

int servo1_channel = 0;  // Front Left
int servo2_channel = 1;  // Front Right
int servo3_channel = 2;  // Back Center

// Convert angle (0-180°) to pulse (150-600)
int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void setServoAngle(int channel, int angle) {
  pwm.setPWM(channel, 0, angleToPulse(angle));
}

void setup() {
  Serial.begin(115200);

  // --- For ESP32: explicitly set SDA and SCL if needed
  Wire.begin(21, 22);  // SDA, SCL (change if using different pins)

  pwm.begin();          // Start PWM driver
  pwm.setPWMFreq(50);   // Set frequency to 50Hz for servos

  delay(10);            // Wait for PCA9685 to stabilize
  setServoAngle(servo1_channel, 90);
  setServoAngle(servo2_channel, 90);
  setServoAngle(servo3_channel, 90);
  delay(3000);
  levelPlate();         // Initialize to neutral position
}

void loop() {
  tiltForward(); delay(2000);
  levelPlate();  delay(1000);

  tiltBackward(); delay(2000);
  levelPlate();   delay(1000);

  tiltLeft();     delay(2000);
  levelPlate();   delay(1000);

  tiltRight();    delay(2000);
  levelPlate();   delay(1000);
}

// --- Tilt Functions ---
void levelPlate() {
  setServoAngle(servo1_channel, 90);
  setServoAngle(servo2_channel, 90);
  setServoAngle(servo3_channel, 90);
}

void tiltForward() {
  setServoAngle(servo1_channel, 80);
  setServoAngle(servo2_channel, 80);
  //setServoAngle(servo3_channel, 100);
}

void tiltBackward() {
  //setServoAngle(servo1_channel, 100);
  //setServoAngle(servo2_channel, 100);
  setServoAngle(servo3_channel, 80);
}

void tiltLeft() {
  setServoAngle(servo1_channel, 80);
  //setServoAngle(servo2_channel, 100);
  setServoAngle(servo3_channel, 90);
}

void tiltRight() {
  //setServoAngle(servo1_channel, 100);
  setServoAngle(servo2_channel, 80);
  setServoAngle(servo3_channel, 90);
}
