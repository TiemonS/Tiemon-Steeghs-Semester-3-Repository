/*
Dit is de code van master 2, deze master print niks uit maar is voor de rest qua functionaliteit hetzelfde als de andere master
Het adres van deze master is 8
*/

#include <Arduino.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#include "bitmaps.hpp"


#define SDA_Pin A4
#define SCL_Pin A5

#define OLED_ADDR 0x3C
#define PIN_RESET 9  
#define DC_JUMPER 1
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration
bool BusAvialable = true;

void drawTest();

void requestBusStatus();
bool isBusAvailable2();
bool readSDA();

void setup()
{
  Serial.begin(9600);
  delay(100);
  Wire.begin(8); //De master verbinden aan de bus met het adres 8
  
  oled.begin();    // Oled instellen
  oled.clear(ALL); // Geheugen van de oled clearen
  oled.display();  // De display laten tonen wat in de buffer zit (standaard splashscreen)
  delay(1000);     // extra delay
  oled.clear(PAGE); // De buffer van de oled clearen
}

void loop()
{
  // Lees de SDA-lijn en print de waarde
  // bool sdaValue = readSDA();
  // Serial.println(sdaValue);

  int SDA_value = digitalRead(SDA);
  if (!SDA_value)
  {
    for (size_t i = 0; i < 8; i++)
    {
      Serial.println(digitalRead(SDA));
    }    
  }
  
  //is de bus beschikbaar?
  // if(isBusAvailable2()) 
  // {
  //   //dan kan er data geschreven worden
  //   BusAvialable = false;
  //   drawTest();
  //   delay(100);
  //   BusAvialable = true;
  //   delay(1000);
  // }
}

void drawTest()
{
    //het geheugen en de buffer van de oled eerst leegmaken
    oled.clear(ALL);
    oled.clear(PAGE);
    //het tekenenen van een test bitmap
    oled.drawBitmap(Test_bitmap);
    //het updaten van de display
    oled.display();
}

void SendTestData() 
{
  oled.data(0);
  if (digitalRead(SDA))
  {
    //als de sda lijn niet laag is, gaat er iets fout (de andere master is bezig)
    return;
  }
  
  

  oled.data(0);
  oled.data(0);
  oled.data(1);
  oled.data(1);
  oled.data(0);
  oled.data(0);
  oled.data(0);
  oled.data(0);
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

//Low code lezen van binnenkomende data
bool readSDA() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // ACK verzenden om SDA-lijn te lezen
  while (!(TWCR & (1 << TWINT))); // Wacht tot I2C-actie is voltooid

  return bit_is_set(TWDR, 0); // Lees de bit 0 van het TWDR-register (SDA-lijn)
}

int CheckSDALine() 
{
  
}