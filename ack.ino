#include <SPI.h>
#include <LoRa.h>

#define MESSAGE_DELAY 1000 // Delay between message transmissions
#define ACK_TIMEOUT 1000   // Timeout for receiving acknowledgment

unsigned long lastTransmissionTime = 0;
bool awaitingAck = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }
}

void loop() {
  if (millis() - lastTransmissionTime >= MESSAGE_DELAY && !awaitingAck) {
    sendMessage("Hello, world!");
    awaitingAck = true;
  }

  // Check for incoming messages
  if (LoRa.parsePacket()) {
    // Read message
    String incomingMessage = "";
    while (LoRa.available()) {
      incomingMessage += (char)LoRa.read();
    }
    Serial.println("Received: " + incomingMessage);

    // Send acknowledgment
    sendAck();
  }

  // Check for acknowledgment timeout
  if (awaitingAck && millis() - lastTransmissionTime >= ACK_TIMEOUT) {
    Serial.println("Acknowledge timeout. Resending message.");
    awaitingAck = false;
  }
}

void sendMessage(String message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  Serial.println("Sent: " + message);
  lastTransmissionTime = millis();
}

void sendAck() {
  LoRa.beginPacket();
  LoRa.print("ACK");
  LoRa.endPacket();
  Serial.println("Sent ACK");
  awaitingAck = false;
}
