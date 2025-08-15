#define PUL_PIN 15  // Pulse pin connected to PUL- on M542 23
#define DIR_PIN 2  // Direction pin connected to DIR- on M542 19

#define PUL_PIN2 23  // Pulse pin connected to PUL- on M542 15
#define DIR_PIN2 19  // Direction pin connected to DIR- on M542 2

#define delay_ms 300
#define delay_ms2 300

void setup() {
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(PUL_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);
}

void loop() {
  // Rotate 100 steps forward
  /*yMove(150, HIGH);
  delay(500);
  yMove(150, LOW);
  delay(1500);*/


  both(300, HIGH);
  delay(1000);
  
  /*yMove(150, HIGH);
  delay(300);
  yMove(150, LOW);
  delay(300);
  both(300, LOW);
  delay(1500);

  yMove(150, LOW);
  delay(300);
  yMove(150, HIGH);
  delay(1500);
  
  both(300, HIGH);
  delay(300);
  yMove(150, LOW);
  delay(300);
  yMove(150, HIGH);
  delay(300);
  both(300, LOW);
  delay(1500);
*/
  

  
  
}

void moveSteps(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(PUL_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(PUL_PIN, LOW);
    delayMicroseconds(500);
  }
}


void yMove(int steps, bool a){
  digitalWrite(DIR_PIN, a);
  moveSteps(steps);
}

void both(int steps, bool a){
  digitalWrite(DIR_PIN, HIGH);
  digitalWrite(DIR_PIN2, LOW);

  for (int i = 0; i < steps; i++) {
    digitalWrite(PUL_PIN, HIGH);
    digitalWrite(PUL_PIN2, HIGH);
    delayMicroseconds(500);
    digitalWrite(PUL_PIN, LOW);
    digitalWrite(PUL_PIN2, LOW);
    delayMicroseconds(500);
  }
}
