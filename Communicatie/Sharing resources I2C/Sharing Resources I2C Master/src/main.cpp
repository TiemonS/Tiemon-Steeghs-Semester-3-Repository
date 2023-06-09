/*
Dit is de code van de "Hoofd master"
Deze master is degene die ook de waardes uitprint die hij terugkrijgt van de andere master
Het adres van deze master is 10
*/

#include <Arduino.h>
#include "bitmaps.hpp"

#include <Wire.h>
#include <SFE_MicroOLED.h>

#define OLED_ADDR 0x3D
#define PIN_RESET 9  
#define DC_JUMPER 1

#define SDA_Pin A4
#define SCL_Pin A5

MicroOLED oled(PIN_RESET, DC_JUMPER);  //Het opzetten van het I2C Oled object
bool BusAvialable = true;

void drawTest();
bool isBusAvailable();
bool isBusAvailable2();
void requestBusStatus();
void SendTestData();

void setup()
{
  delay(100); //een kleine delay om de display de tijd te geven om goed op te starten
  Serial.begin(9600);
  
  pinMode(SDA_Pin, INPUT_PULLUP);
  Wire.begin(10); //De master verbinden aan de bus met het adres 10

  oled.begin();    // Oled instellen
  oled.clear(ALL); // Geheugen van de oled clearen
  oled.display();  // De display laten tonen wat in de buffer zit (standaard splashscreen)
  delay(1000);     // extra delay
  oled.clear(PAGE); // De buffer van de oled clearen
}

void loop()
{
  
 SendTestData();

  // if(requestavialable) 
  // {
  //   BusAvialable = false;
  //   drawTest();
  //   delay(100);
  //   BusAvialable = true;
  //   delay(500);
  // }

 delay(1000);
}

void drawTest()
{
    //het geheugen en de buffer van de oled eerst leegmaken
    oled.clear(ALL);
    oled.clear(PAGE);
    //het tekenenen van een test bitmap
    oled.drawBitmap(bender);
    //het updaten van de display
    oled.display();
}

void SendTestData() 
{
  oled.data(0);

  oled.data(0);
  oled.data(0);
  oled.data(1);
  oled.data(1);
  oled.data(0);
  oled.data(0);
  oled.data(0);
  oled.data(0);
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
  Wire.beginTransmission(8);  // transmission starten met master 2 (adres 8)
  Wire.write(1);  // aangeven aan de andere master 2 dat we de status willen opvragen
  Wire.endTransmission(); //transmission weer stoppen

  Wire.requestFrom(8, 1);  // De satus opvragen 
  //is er data beschikbaar?
  if (Wire.available()) {
  //lees de data en sla het op
    int status = Wire.read();
    return status == 1;
  }

  return false;
}



