#include <SPI.h>
#include <LoRa.h>

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

  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println("LoRa Receiver Callback");

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
  digitalWrite(LED_BUILTIN, HIGH);

  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++)
  {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());

  digitalWrite(LED_BUILTIN, LOW);
}