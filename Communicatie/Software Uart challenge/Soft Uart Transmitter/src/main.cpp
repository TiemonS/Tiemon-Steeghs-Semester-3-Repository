#include <Arduino.h>

#define TX_PIN 2
#define BAUD_RATE 9600
#define BIT_DELAY_US 104 // = 1000000 / BAUD_RATE / 10 (10 bits per byte)
String data = "t";

unsigned long CurrentMicros;

void SendCharacter(byte ch);
void Micros_Bit_Delay();

void setup() {
  pinMode(TX_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for (unsigned int i = 0; i < data.length(); i++) {
    byte ch = data.charAt(i);
    SendCharacter(ch);
  }
}

void SendCharacter(byte ch) 
{
  //Eerst de begin tijd opslaag
  CurrentMicros = micros();
  digitalWrite(TX_PIN, LOW); // start bit
  
  //vervolgens wachten totdat de tijd van de digitalwrite + de wachttijd gelijk is aan 104 ms
  Micros_Bit_Delay();

  for (int j = 0; j < 8; j++) {
    CurrentMicros = micros();
    digitalWrite(TX_PIN, ch & 1); // send bit
    ch >>= 1;
    Micros_Bit_Delay();
  }
  CurrentMicros = micros();
  digitalWrite(TX_PIN, HIGH); // stop bit
  Micros_Bit_Delay();

  delay(500);
}

void Micros_Bit_Delay() 
{
  while (micros() < CurrentMicros + BIT_DELAY_US)
  {
    //Wachten totdat de 104 ms voorbij zijn
  }
}