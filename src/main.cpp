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

  Serial.begin(115200);
  while (!Serial)
    ;

  Serial1.begin(9600);

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

  // read packet
  for (int i = 0; i < packetSize; i++)
  {
    Serial1.print((char)LoRa.read());
  }
  Serial1.print("\n");

  // print RSSI of packet
  Serial.print("Received packet with RSSI ");
  Serial.println(LoRa.packetRssi());

  digitalWrite(LED_BUILTIN, LOW);
}