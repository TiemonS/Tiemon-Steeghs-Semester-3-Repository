/*
Dit is de code van master 2, deze master print niks uit maar is voor de rest qua functionaliteit hetzelfde als de andere master
Het adres van deze master is 8
*/

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
  Wire.begin(8); //De master verbinden aan de bus met het adres 8
  Wire.onRequest(requestBusStatus); //de onrequest methode instellen

  oled.begin();    // Oled instellen
  oled.clear(ALL); // Geheugen van de oled clearen
  oled.display();  // De display laten tonen wat in de buffer zit (standaard splashscreen)
  delay(1000);     // extra delay
  oled.clear(PAGE); // De buffer van de oled clearen
}

void loop()
{
  //is de bus beschikbaar?
  if(isBusAvailable2()) 
  {
    //dan kan er data geschreven worden
    BusAvialable = false;
    drawTest();
    delay(100);
    BusAvialable = true;
    delay(1000);
  }
}

void drawTest()
{
    //het geheugen en de buffer van de oled eerst leegmaken
    oled.clear(ALL);
    oled.clear(PAGE);
    //het tekenenen van een test bitmap
    oled.drawBitmap(marco_borsato);
    //het updaten van de display
    oled.display();
}

//eerste versie voor het kijken van of de bus beschikbaar is
//werkt alleen bij het aansluiten van een nieuwe master aan de bus
bool isBusAvailable() {
  Wire.beginTransmission(OLED_ADDR);
  bool success = Wire.endTransmission() == 0;
  return success;
}

//vraagt aan de andere master of de bus vrij is
bool isBusAvailable2() {
  Wire.beginTransmission(10);  // transmission starten met master 2 (adres 10)
  Wire.write(1);  // aangeven aan de andere master 2 dat we de status willen opvragen
  Wire.endTransmission(); //transmission weer stoppen

  Wire.requestFrom(10, 1);  // De satus opvragen 
  //is er data beschikbaar?
  if (Wire.available()) {
  //lees de data en sla het op
    int status = Wire.read();
    return status == 1;
  }

  return false;
}

void requestBusStatus() {
  Wire.write(BusAvialable);
}