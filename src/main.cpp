/*
 * feather32u4_rfm9x-arduino-lora433RX
 * 
 * Firmware for the LoRa receiver. 
 * Receives packet from LoRa transmitter, packages as a string, and send to Serial1 port.
 */

#include <SPI.h>
#include <LoRa.h>

#define FIRMWARE_FILENAME "feather32u4_rfm9x-arduino-lora433RX"
#define FIMWARE_VERSION 1.0

// pin assignments
#define LED_BUILTIN 13

// feather32u4 pins
#define NSS 8
#define NRESET 4
#define DIO0 7

void onReceive(int packetSize);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  LoRa.setPins(NSS, NRESET, DIO0);

  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("===========================================================");
  Serial.println();
  Serial.print("Kingswood LoRa Receiver v.");
  Serial.println(FIMWARE_VERSION);
  Serial.println();
  Serial.print("Firmware filename : ");
  Serial.println(FIRMWARE_FILENAME);
  Serial.println();
  Serial.println("===========================================================");
  Serial.println();

  Serial1.begin(9600);

  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop()
{
  // do nothing
}

void onReceive(int packetSize)
{
  // received a packet - flash the LED
  digitalWrite(LED_BUILTIN, HIGH);

  // construct a string from the packet bytes
  char *buf = malloc(sizeof(char) * packetSize + 1);
  int i;
  for (i = 0; i < packetSize; i++)
  {
    buf[i] = ((char)LoRa.read());
  }
  buf[i] = '\0';

  // send it
  Serial1.print(buf);

  // print RSSI of packet
  Serial.print("Received packet: ");
  Serial.print(buf);
  Serial.print(" with RSSI ");
  Serial.println(LoRa.packetRssi());

  free(buf);

  digitalWrite(LED_BUILTIN, LOW);
}