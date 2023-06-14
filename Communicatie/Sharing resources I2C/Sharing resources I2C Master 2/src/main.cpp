/*
Dit is de code van master 2, deze master print niks uit maar is voor de rest qua functionaliteit hetzelfde als de andere master
Het adres van deze master is 8
*/

#include <Arduino.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#include "bitmaps.hpp"


#define SDA_Pin 18
#define SCL_Pin A5
#define I2C_DELAY 317

#define OLED_ADDR 0x3C
#define PIN_RESET 9  
#define DC_JUMPER 1
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration
bool BusAvialable = true;
unsigned long CurrentMicros;

void drawTest();

void requestBusStatus();
bool isBusAvailable2();
bool readSDA();
bool SendAndCheckSDALine(int SendBit);
String SendTestData();
void SendTestData2();
void Micros_Bit_Delay();
bool customDigitalRead();

volatile uint8_t *portReg;
volatile uint8_t *pinReg;
uint8_t sdaBitMask;

void setup()
{
  Serial.begin(9600);
  delay(100);
  Wire.begin(8); //De master verbinden aan de bus met het adres 8
  
  pinMode(SDA_Pin, INPUT);

  oled.begin();    // Oled instellen
  oled.clear(ALL); // Geheugen van de oled clearen
  oled.display();  // De display laten tonen wat in de buffer zit (standaard splashscreen)
  delay(1000);     // extra delay
  oled.clear(PAGE); // De buffer van de oled clearen
}

void loop()
{
  // Lees de SDA-lijn en print de waarde
  int SDA_value = (PINC & (1 << PC4)) != 0;
  if (!SDA_value)
  {
    for (size_t i = 0; i < 8; i++)
    {
      delayMicroseconds(317);
      Serial.println(digitalRead(SDA));
    }    
  }
  
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

void SendTestData2() 
{

  oled.data(0);
  oled.data(0);
  oled.data(0);
  oled.data(1);
  oled.data(1);
  oled.data(1);
  oled.data(0);
  oled.data(0);
  oled.data(0);
}

String SendTestData() 
{
  if (!SendAndCheckSDALine(0))
  {
    return "Error in sending LOW SDA bit!";
  }
  if (!SendAndCheckSDALine(1))
  {
    return "Error in sending HIGH SDA bit!";
  }
  if (!SendAndCheckSDALine(1))
  {
    return "Error in sending HIGH SDA bit!";
  }
  if (!SendAndCheckSDALine(0))
  {
    return "Error in sending LOW SDA bit!";
  }

  return "Data send succesvol!";
}

//Low code lezen van binnenkomende data
bool readSDA() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // ACK verzenden om SDA-lijn te lezen
  while (!(TWCR & (1 << TWINT))); // Wacht tot I2C-actie is voltooid

  return bit_is_set(TWDR, 0); // Lees de bit 0 van het TWDR-register (SDA-lijn)
}

bool SendAndCheckSDALine(int SendBit) 
{
  //als eerste verstuur de bit
  oled.data(SendBit);
  if (digitalRead(SDA) == SendBit)
  {
    //de bit komt overheen dus er is geen probleem
    return true;
  }
  else  
  {
    //de sda lijn heeft een andere waarde dus er is een probleem
    return false;
  }
  
}

