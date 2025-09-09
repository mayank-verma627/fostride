#define EN_PIN   27
#define DIR_PIN  25
#define STEP_PIN 26

void setup() {
  pinMode(EN_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  digitalWrite(EN_PIN, LOW);   // Enable driver
  digitalWrite(DIR_PIN, HIGH); // Set initial direction
}

void loop() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(200);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(200);
}
