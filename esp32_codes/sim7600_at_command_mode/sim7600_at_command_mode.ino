#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>

#define MODEM_RX 16
#define MODEM_TX 17
#define MODEM_BAUD 115200

HardwareSerial SerialAT(1);  // UART1 for SIM7600
TinyGsm modem(SerialAT);

void setup() {
  Serial.begin(115200);
  SerialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);

  Serial.println("Initializing SIM7600...");
  modem.restart();
  
  if (modem.isNetworkConnected()) {
    Serial.println("Network connected!");
  } else {
    Serial.println("Failed to connect to network");
  }
}

void loop() {
  // Your code here
}
