#include <Arduino.h>
#include "bitmaps.hpp"

#include <Wire.h>
#include <SFE_MicroOLED.h>

#define OLED_ADDR 0x3D
#define PIN_RESET 9  
#define DC_JUMPER 1
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration
bool BusAvialable = true;

void drawTest();
bool isBusAvailable();
bool isBusAvailable2();
void requestBusStatus();

void setup()
{
  Serial.begin(9600);
  delay(100);
  Wire.begin(10);
  Wire.onRequest(requestBusStatus);
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.
}

void loop()
{
  bool requestavialable = isBusAvailable2();
  Serial.println(requestavialable);
  if(requestavialable) 
  {
    BusAvialable = false;
    drawTest();
    delay(100);
    BusAvialable = true;
    delay(500);
  }
  //delay(1000);
}

void drawTest()
{
    //het geheugen van de oled eerst leegmaken
    oled.clear(ALL);
    oled.clear(PAGE);
    //het tekenenen van een test bitmap
    oled.drawBitmap(bender);//Display Logo
    //het updaten van de display
    oled.display();
}

//werkt alleen bij het aansluiten van een nieuwe master aan de bus
bool isBusAvailable() {
  Wire.beginTransmission(OLED_ADDR);
  bool success = Wire.endTransmission() == 0;
  return success;
}

//vraagt aan de andere master of de bus vrij is
bool isBusAvailable2() {
  Wire.beginTransmission(8);  // Address of Arduino Master 2
  Wire.write(1);  // Request bus availability status
  Wire.endTransmission();

  Wire.requestFrom(8, 1);  // Address of Arduino Master 2
  if (Wire.available()) {
    int status = Wire.read();
    return status == 1;
  }

  return false;
}

void requestBusStatus() {
  Wire.write(BusAvialable);
}
