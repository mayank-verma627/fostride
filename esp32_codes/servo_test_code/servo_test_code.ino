#include <ESP32Servo.h>

Servo myservo;  
int pos = 90;  // start at 90 degrees

void setup() {
  myservo.attach(18);   // attach servo to GPIO 18 (change pin if needed)
  myservo.write(pos);   // set initial position
  delay(1000);
}

void loop() {
  // Move from 90 → 0
  for (pos = 90; pos >= 0; pos--) {
    myservo.write(pos);
    delay(5);
  }
  delay(500);

  // Move from 0 → 90
  for (pos = 0; pos <= 90; pos++) {
    myservo.write(pos);
    delay(5);
  }
  delay(500);

  // Move from 90 → 180
  for (pos = 90; pos <= 180; pos++) {
    myservo.write(pos);
    delay(5);
  }
  delay(500);

  // Move from 180 → 90
  for (pos = 180; pos >= 90; pos--) {
    myservo.write(pos);
    delay(5);
  }
  delay(1000);  // wait before repeating
}
