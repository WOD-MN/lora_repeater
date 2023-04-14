#include <SPI.h>
#include <LoRa.h>

#define SS_PIN 10
#define RST_PIN 9
#define DIO0_PIN 2

#define RX_FREQUENCY 433000000
#define TX_FREQUENCY 433500000
#define BANDWIDTH 125000
#define SPREADING_FACTOR 7
#define CODING_RATE 5
#define SYNC_WORD 0x34

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(RX_FREQUENCY)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }

  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(CODING_RATE);
  LoRa.setSyncWord(SYNC_WORD);
  LoRa.enableCrc();

  LoRa.setFrequency(TX_FREQUENCY); // set the transmission frequency
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet (RSSI=");
    Serial.print(LoRa.packetRssi());
    Serial.print(", SNR=");
    Serial.print(LoRa.packetSnr());
    Serial.print("): ");

    uint32_t crc = LoRa.packetRxCrc();
    if (crc) {
      Serial.print("CRC failed! ");
    }

    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.println();

    LoRa.beginPacket();
    LoRa.write("ACK");
    LoRa.endPacket();
  }
}
