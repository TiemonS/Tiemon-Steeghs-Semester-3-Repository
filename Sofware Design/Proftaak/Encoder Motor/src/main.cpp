#include <Arduino.h>

const int EncoderPinA = 2;
const int EncoderPinB = 8;

uint8_t count = 0; // A volatile variable to store the count of interrupts

void interruptHandler() {
  count++; // Increment the count of interrupts
}

void setup() {
  pinMode(EncoderPinA, INPUT_PULLUP); // Set pin 7 as input with internal pull-up resistor enabled
  pinMode(EncoderPinB, INPUT_PULLUP); // Set pin 8 as input with internal pull-up resistor enabled
  attachInterrupt(digitalPinToInterrupt(EncoderPinA), interruptHandler, FALLING); // Attach interrupt handler to pin 7, which triggers on both falling and rising edges
  //attachInterrupt(digitalPinToInterrupt(EncoderPinB), interruptHandler, CHANGE); // Attach interrupt handler to pin 8, which triggers on both falling and rising edges
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  // Print the count of interrupts every second
  delay(500);
  Serial.print("Count:");
  Serial.println(count);
  Serial.print("Pin value:");
  Serial.println(digitalRead(EncoderPinA));
  //interruptHandler();
}

