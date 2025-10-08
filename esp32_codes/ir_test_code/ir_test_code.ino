int sensorPin = 23;  // connect OUT (black wire) to GPIO 23
int sensorValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
}

void loop() {
  sensorValue = digitalRead(sensorPin);

  if (sensorValue == LOW) {
    Serial.println("Object detected!");
  } else {
    Serial.println("No object");
  }

  delay(200);
}
