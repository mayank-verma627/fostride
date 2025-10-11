#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);  // USB debug
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // UART2

  Serial.println("UART2 Loopback Test Starting...");
  delay(1000);
}

void loop() {
  Serial2.println("Hello from UART2");

  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    Serial.print("Received on UART2: ");
    Serial.println(data);
  }

  delay(1000);
}
