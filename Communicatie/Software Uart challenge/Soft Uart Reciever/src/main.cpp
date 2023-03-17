#include <Arduino.h>
#include "SoftwareSerial.h"

//SOFTSERIEL TUSSEN ARDUINO'S
#define RX_PIN 0
#define BAUD_RATE 9600
#define BIT_DELAY_US 104 // = 1000000 / BAUD_RATE / 10 (10 bits per byte)

String receivedData = "";

void ProcessData();

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(RX_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  
  if (digitalRead(RX_PIN) == LOW) 
  {
    // start bit detected
    Serial.println("Start bit recieved!");
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(BIT_DELAY_US);
    ProcessData();
    Serial.println(receivedData);
  }
  
}

void ProcessData() 
{
    byte receivedByte = 0;

    //checking the data
    for (int i = 0; i < 8; i++) {
      receivedByte |= (digitalRead(RX_PIN) << i);
      delayMicroseconds(BIT_DELAY_US);
    }

    if (digitalRead(RX_PIN) == HIGH) {
      // stop bit detected
      Serial.println("Stopbit recieved!");
      //digitalWrite(LED_BUILTIN, LOW);

      //recieved character is added to the array
      receivedData += (char)receivedByte;
    }
}