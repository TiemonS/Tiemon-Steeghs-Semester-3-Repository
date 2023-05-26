#include <Arduino.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#include "bitmaps.hpp"

#define OLED_ADDR 0x3C
#define PIN_RESET 9  
#define DC_JUMPER 1
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration
bool BusAvialable = true;

void drawTest();

void requestBusStatus();
bool isBusAvailable2();

void setup()
{
  delay(100);
  Wire.begin(8);
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.
  Wire.onRequest(requestBusStatus);
}

void loop()
{
  //Serial.println(isBusAvailable());
  if(isBusAvailable2()) 
  {
    BusAvialable = false;
    drawTest();
    delay(100);
    BusAvialable = true;
    delay(1000);
  }
}

void drawTest()
{
    //het geheugen van de oled eerst leegmaken
    oled.clear(ALL);
    oled.clear(PAGE);
    //het tekenenen van een test bitmap
    oled.drawBitmap(marco_borsato);//Display Logo
    //het updaten van de display
    oled.display();
}

bool isBusAvailable() {
  Wire.beginTransmission(OLED_ADDR);
  bool success = Wire.endTransmission() == 0;
  return success;
}

//vraagt aan de andere master of de bus vrij is
bool isBusAvailable2() {
  Wire.beginTransmission(10);  // Address of Arduino Master 2
  Wire.write(1);  // Request bus availability status
  Wire.endTransmission();

  Wire.requestFrom(10, 1);  // Address of Arduino Master 2
  if (Wire.available()) {
    int status = Wire.read();
    return status == 1;
  }

  return false;
}

void requestBusStatus() {
  Wire.write(BusAvialable);
}