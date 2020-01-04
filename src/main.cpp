/*
 * feather32u4_rfm9x-arduino-lora433RX
 * 
 * Firmware for the LoRa receiver. 
 * Receives packet from LoRa transmitter. 
 * Deserialises. 
 * Adds in LoRa signal quality data. 
 * Reserialises and sends to Serial1 port.
 */

#include <SPI.h>
#include <LoRa.h>

#include "main.h"
#include "utils.h"

void onReceive(int packetSize);

void setup()
{
  // start the serial ports
  Serial.begin(115200);
  Serial1.begin(9600);
  delay(2000);

  // print the firmware banner information
  printBanner(FIRMWARE_TITLE, FIRMWARE_FILENAME, FIRMWARE_VERSION, DEVICE_ID);

  // set the radio pins
  LoRa.setPins(NSS, NRESET, DIO0);

  // start the radio
  Serial.println("Starting radio");
  if (!LoRa.begin(433E6))
  {
    while (1)
      Serial.print('.');
  }
  Serial.println("Radio started");

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
  Serial.print("RX packet: ");
  Serial.print(buf);
  Serial.print(" with RSSI ");
  Serial.println(LoRa.packetRssi());

  free(buf);

  digitalWrite(LED_BUILTIN, LOW);
}