/*
  This header includes helper functions for working with a BN-880Q GPS module through TinyGPSPlus and QMC5883LCompass, a common Arduino library
  It is assumed that a connection is made between pin 0 RX on the Arduino and the TX of the GPS module
*/

#include "TinyGPSPlus.h"
#include <QMC5883LCompass.h>

// The TinyGPSPlus object and the QMC5883LCompass compass objects
TinyGPSPlus gps;
QMC5883LCompass compass;

//  The delay in milliseconds between the last message coming in and the return string indicating a lack of connection to a GPS unit
unsigned long lastMessageMillis = 0;
unsigned long millisBeforeConnectionFailed = 5000;
const String noConnectionMessage = "No GPS detected: check wiring.";
const unsigned int latLongDecimals = 9;

//  The Compass data
unsigned int CompassAzimuth = 0;
String CompassDirection = "";

namespace GPSHelper {
  void Initialize() {
    compass.setCalibrationOffsets(-362.00, 236.00, 488.00);
    compass.setCalibrationScales(0.90, 0.77, 1.66);
    compass.init();
  }

  void CompassUpdate() {
    compass.read();
    int rawAzimuth = compass.getAzimuth();
    if (rawAzimuth < 0) rawAzimuth = (360 + rawAzimuth);
    double azimuthPercent = (double)rawAzimuth / 255.0;
    CompassAzimuth = min((unsigned int)(360.0 * azimuthPercent), 360);

    char myArray[3];
    compass.getDirection(myArray, rawAzimuth);
    CompassDirection = "(";
    CompassDirection += String(rawAzimuth);
    CompassDirection += ") ";
    CompassDirection.concat(myArray[0]);
    CompassDirection.concat(myArray[1]);
    CompassDirection.concat(myArray[2]);
  }

  void onLoop() {
    CompassUpdate();

    if ((millis() - lastMessageMillis) > millisBeforeConnectionFailed && gps.charsProcessed() < 10)
    {
      lastMessageMillis = millis();
      Serial.println(F("No GPS detected: check wiring."));
    }
  }

  bool IsLocationValid() {
    return gps.location.isValid();
  }

  bool IsLocationUpdated() {
    return gps.location.isUpdated();
  }

  uint32_t GetLocationFixAge() {
    return gps.location.age();
  }

  String GetBillionthsDecimalString(uint32_t billionths)
  {
    String decimalString = "";
    if (billionths < 100000000) decimalString += "0";
    if (billionths < 10000000) decimalString += "0";
    if (billionths < 1000000) decimalString += "0";
    if (billionths < 100000) decimalString += "0";
    if (billionths < 10000) decimalString += "0";
    if (billionths < 1000) decimalString += "0";
    if (billionths < 100) decimalString += "0";
    if (billionths < 10) decimalString += "0";
    decimalString += billionths;
    return decimalString;
  }

  String GetLocationString() {
    if (!IsLocationValid()) return "[INVALID]";
    else {
      String rawLatAndLong = "[";
      rawLatAndLong += (gps.location.rawLat().negative ? "-" : "+");
      rawLatAndLong += (gps.location.rawLat().deg);
      rawLatAndLong += ".";
      rawLatAndLong += GetBillionthsDecimalString(gps.location.rawLat().billionths);
      rawLatAndLong += ", ";
      rawLatAndLong += (gps.location.rawLng().negative ? "-" : "+");
      rawLatAndLong += (gps.location.rawLng().deg);
      rawLatAndLong += ".";
      rawLatAndLong += GetBillionthsDecimalString(gps.location.rawLng().billionths);
      rawLatAndLong += "]";
      return rawLatAndLong;
    }
  }

  bool IsDateValid() {
    return gps.date.isValid();
  }

  String GetDateString() {
    if (!IsDateValid()) return "[INVALID]";
    String dateString = "[";
    dateString += gps.date.month();
    dateString += "/";
    dateString += gps.date.day();
    dateString += "/";
    dateString += gps.date.year();
    dateString += "]";
    return dateString;
  }

  bool IsTimeValid() {
    return gps.time.isValid();
  }

  String GetTimeString() {
    if (!IsTimeValid()) return "[INVALID]";
    String timeString = "[";
    timeString += (gps.time.hour() < 10) ? "0" : "";
    timeString += (gps.time.hour());
    timeString += ":";
    timeString += (gps.time.minute() < 10) ? "0" : "";
    timeString += (gps.time.minute());
    timeString += ":";
    timeString += (gps.time.second() < 10) ? "0" : "";
    timeString += (gps.time.second());
    timeString += "]";
    return timeString;
  }

  bool IsSpeedValid() {
    return gps.speed.isValid();
  }

  String GetSpeedString() {
    if (!IsSpeedValid()) return "[INVALID]";
    return "[" + String(gps.speed.mph()) + "mph]";
  }

  bool IsCourseValid() {
    return gps.course.isValid();
  }

  String GetCourseString() {
    if (!IsCourseValid()) return "[INVALID]";
    return "[" + String(gps.course.deg()) + " degrees]";
  }

  bool IsSatellitesValid() {
    return gps.satellites.isValid();
  }

  String GetSatellitesString() {
    if (!IsSatellitesValid()) return "(INVALID)";
    return "(Satellites Value: " + String(gps.satellites.value()) + ")";
  }

  bool IsHDOPValid() {
    return gps.hdop.isValid();
  }

  String GetHdopString() {
    if (!IsHDOPValid()) return "(INVALID)";
    return "{HDOP Value: " + String(gps.hdop.hdop()) + "}";
  }
}

void PrintSensorData() {
  if (GPSHelper::IsLocationUpdated())
  {
    String fullUpdate = "";
    fullUpdate += ("Location: "); 
    fullUpdate += ("RAW" + GPSHelper::GetLocationString() + ", ");
    fullUpdate += ("Date/Time: " + GPSHelper::GetDateString() + " | " + GPSHelper::GetTimeString() + " ~ ");

    fullUpdate += ("(Fix Age: ");
    fullUpdate += (GPSHelper::GetLocationFixAge());
    fullUpdate += ("ms) | ");

    fullUpdate += ("Speed: " + GPSHelper::GetSpeedString() + " || ");
    //fullUpdate += ("Course: " + GPSHelper::GetCourseString() + " ||");

    fullUpdate += ("Satellites: " + GPSHelper::GetSatellitesString() + " ~ ");
    fullUpdate += ("HDOP: " + GPSHelper::GetHdopString() + " - ");

    fullUpdate += ("Compass Facing: " + CompassDirection);

    Serial.println(fullUpdate);
  }
}