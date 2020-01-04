#pragma once
#include <Arduino.h>
#include "utils.h"

void printBanner(const char *firmware_title, const char *firmware_filename, const char *firmware_version, const char *deviceID)
{
    Serial.println();
    Serial.println("======================================================================");
    Serial.println(firmware_title);
    Serial.println("----------------------------------------------------------------------");
    Serial.print("Firmware filename   : ");
    Serial.print(firmware_filename);
    Serial.print(" V.");
    Serial.println(firmware_version);
    Serial.print("Sensor ID           : ");
    Serial.println(deviceID);
    Serial.println("======================================================================");
    Serial.println();
}