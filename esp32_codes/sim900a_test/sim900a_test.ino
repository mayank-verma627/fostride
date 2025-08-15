#include <SoftwareSerial.h>

// Define RX and TX pins for SoftwareSerial
SoftwareSerial SIM900A(7, 8); // RX = Pin 7, TX = Pin 8

void setup() {
  // Initialize serial communication
  Serial.begin(115200); // For communication with the PC
  SIM900A.begin(9600); // For communication with the SIM900A module

  // Wait for the module to initialize
  delay(1000);
  Serial.println("Initializing SIM900A...");

  // Send AT command to check module response
  SIM900A.println("AT");
  delay(1000);
  while (SIM900A.available()) {
    Serial.write(SIM900A.read()); // Print module response to Serial Monitor
  }

  // Send SMS command
  SIM900A.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  SIM900A.println("AT+CMGS=\"+919082604406\""); // Replace with recipient's phone number
  delay(1000);
  SIM900A.println("Hello, this is a test SMS from SIM900A!"); // SMS content
  delay(1000);
  SIM900A.write(26); // Send Ctrl+Z to send the SMS
  delay(1000);
  Serial.println("SMS sent!");
}

void loop() {
  // No actions in the loop
}
