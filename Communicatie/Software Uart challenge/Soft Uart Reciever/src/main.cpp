#include <Arduino.h>
#include "SoftwareSerial.h"

#define RX_TESTPIN 0

//SOFTSERIEL TUSSEN ARDUINO'S
#define RX_PIN 3
#define BAUD_RATE 9600
#define BIT_DELAY_US 104 // = 1000000 / BAUD_RATE / 10 (10 bits per byte)

String receivedData = "";
int CurrentMicros;

void ProcessData();

void Micros_Bit_Delay(); 

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(RX_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  
  CurrentMicros = micros();
  if (digitalRead(RX_PIN) == LOW) 
  {
    // start bit gedetecteerd
    //Serial.println("Start bit recieved!");
    Micros_Bit_Delay();
    ProcessData();
    //Serial.println(receivedData);
  }
}

void ProcessData() 
{
    char receivedByte = 0;

    //data controlleren
    for (int i = 0; i < 8; i++) {
      CurrentMicros = micros();
      receivedByte |= (digitalRead(RX_PIN) << i);
      Micros_Bit_Delay();
    }

    CurrentMicros = micros();
    if (digitalRead(RX_PIN) == HIGH) {
      // stop bit detected
      //Serial.println("Stopbit recieved!");
      //digitalWrite(LED_BUILTIN, LOW);
      Serial.println(receivedByte);

      //recieved character is added to the array
      receivedData += receivedByte;
      Micros_Bit_Delay();
    }
    else 
    {
      Serial.println("Invalid data!");
      return;
    }
}

void Micros_Bit_Delay() 
{
  while (micros() < CurrentMicros + BIT_DELAY_US)
  {
    //Wachten totdat de 104 ms voorbij zijn
  }
}