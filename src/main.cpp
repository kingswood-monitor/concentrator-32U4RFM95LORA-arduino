/*
 * kingswood-monitor-lora-concentrator
 * 
 * Firmware for the LoRa receiver. 
 * Receives packet from LoRa transmitter. 
 * Deserialises. 
 * Adds in LoRa signal quality data. 
 * Reserialises and sends to Serial1 port.
 */

#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#include "utils.h"

#define FIRMWARE_TITLE "Kingswood Monitor LoRa Concentrator"
#define FIRMWARE_FILENAME "kingswood-monitor-lora-concentrator"
#define FIRMWARE_VERSION "1.1"
#define DEVICE_ID "FEATHER32U4LORA-002"

// pin assignments
#define LED_BUILTIN 13 // HIGH is ON

// feather32u4 LoRa pin assignments
#define NSS 8
#define NRESET 4
#define DIO0 7

#define DEBUG false // supress serial printing if false

void onReceive(int packetSize);

// Create the empty JSON document - https://arduinojson.org/v6/assistant/
// Note: it's larger than the arriving document because we are adding LoRa info
const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4);
StaticJsonDocument<capacity> doc;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial1.begin(9600);
  delay(2000);

  printBanner(FIRMWARE_TITLE, FIRMWARE_FILENAME, FIRMWARE_VERSION, DEVICE_ID);

  LoRa.setPins(NSS, NRESET, DIO0);

  Serial.println("Starting radio");
  if (!LoRa.begin(433E6))
  {
    while (1)
      Serial.print('.');
  }
  Serial.println("Radio started");

  LoRa.onReceive(onReceive);

  LoRa.receive();
}

void loop()
{
  // do nothing
}

void onReceive(int packetSize)
{

  // construct a string from the packet bytes
  char *buf = (char *)malloc(sizeof(char) * packetSize + 1);
  int i;
  for (i = 0; i < packetSize; i++)
  {
    buf[i] = ((char)LoRa.read());
  }
  buf[i] = '\0';

  // deserialise it
  DeserializationError error = deserializeJson(doc, buf);
  if (error)
  {
    return;
  }

  // add LoRa quality data
  JsonObject obj = doc["status"].as<JsonObject>();
  JsonObject status_lora = obj.createNestedObject("lora");
  status_lora["packetRssi"] = LoRa.packetRssi();
  status_lora["packetSnr"] = LoRa.packetSnr();
  status_lora["packetFrequencyError"] = LoRa.packetFrequencyError();

  // reserialise it
  char output[255];
  serializeJson(doc, output);

  // send it
  Serial1.print(output);

  // report it
  if (DEBUG)
  {
    Serial.println("---");
    serializeJsonPretty(doc, Serial);
    Serial.println();
  }

  // flash the LED
  Serial.println("Flashing");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);

  // clear memory
  free(buf);
}