#include "GPSHelper.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}

  Serial3.begin(9600);
  while (!Serial3) {}

  PrintProgramIntro();
}

void loop() {
  GPSHelper::onLoop();

  if (Serial3.available()) {
    if (gps.encode(Serial3.read())) {
      PrintSensorData();
    }
  }
}

void PrintProgramIntro() {
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("[GPSTest.ino]: A simple demonstration of TinyGPSPlus and QMC5883LCompass using my GPSHelper.h with an attached BN-880Q GPS module");
  Serial.println("Testing TinyGPSPlus library v. " + String(TinyGPSPlus::libraryVersion()) + " by Mikal Hart");
}