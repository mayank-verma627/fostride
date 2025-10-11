// Pin connections
const int TRIG_PIN = 18;    // Trigger Pin
const int ECHO_PIN = 19;    // Echo Pin
const float THRESHOLD_CM = 25.0;

// UART2 Pins
#define ESP32_TX2 17
#define ESP32_RX2 16

void setup() {
  Serial.begin(115200);  // Serial Monitor
  Serial2.begin(115200, SERIAL_8N1, ESP32_RX2, ESP32_TX2);  // UART2

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Setup complete. Starting sensor and UART2...");
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo duration
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  float distance = (duration / 2.0) / 29.1;
  Serial.print("Distance: ");
  Serial.println(distance);

  // Compare distance and send result over UART2
  if (distance <= THRESHOLD_CM) {
    Serial.println(true);
    Serial2.println(true);
  } else {
    Serial.println(false);
    Serial2.println(false);
  }

  // Read and print data received from UART2 (if any)
  while (Serial2.available()) {
    String received = Serial2.readStringUntil('\n');
    Serial.print("Received from UART2: ");
    Serial.println(received);
  }

  delay(500);  // Delay between readings
}
