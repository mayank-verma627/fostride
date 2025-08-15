#define STEP_PIN 22
#define DIR_PIN 23
#define EN_PIN 0

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  
  digitalWrite(EN_PIN, LOW); // Enable driver
  digitalWrite(DIR_PIN, HIGH); // Set direction
}

void loop() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(500);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(500);
}
