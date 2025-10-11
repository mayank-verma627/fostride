#define EN_PIN   33
#define DIR_PIN  15
#define STEP_PIN 2

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
