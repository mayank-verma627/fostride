// ESP32 GSM SIM900A SMS Send/Receive Example
// Using Serial2 (Hardware UART)

#define RXD2 16   // ESP32 RX pin connected to SIM900A TX
#define TXD2 17   // ESP32 TX pin connected to SIM900A RX

void setup() {
  Serial.begin(115200);      // Serial Monitor
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Serial2 for SIM900A

  Serial.println("SIM900A Ready");
  delay(100);
  Serial.println("Type s to send message or r to receive message");
}

void loop() {
  // Handle Serial Monitor input
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 's') {
      sendMessage();
    } 
    else if (cmd == 'r') {
      receiveMessage();
    }
  }

  // Forward data from SIM900A to Serial Monitor
  if (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
}

void sendMessage() {
  Serial.println("Sending Message...");
  Serial2.println("AT+CMGF=1");    // Text mode
  delay(1000);

  Serial2.println("AT+CMGS=\"+919082604406\""); // Replace with recipient's number
  delay(1000);

  Serial2.println("Good morning, how are you doing?"); // Message text
  delay(500);

  Serial2.write(26); // CTRL+Z
  delay(2000);

  Serial.println("Message sent!");
}

void receiveMessage() {
  Serial.println("Reading SMS...");
  delay(1000);
  Serial2.println("AT+CNMI=2,2,0,0,0"); // Live SMS display
  delay(1000);
  Serial.println("Unread Message check done.");
}
