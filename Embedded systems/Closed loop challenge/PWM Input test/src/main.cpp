#include <Arduino.h>

const int pwmPin = 6;      // PWM output pin
const int pwmFreq = 1000;  // PWM frequency in Hz

void setup() {
  pinMode(pwmPin, OUTPUT);  // Set PWM pin as output
}

void loop() {
  // Simulate a PWM signal with varying duty cycle
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle += 5) {
    analogWrite(pwmPin, dutyCycle);  // Set the duty cycle
    delay(10);                      // Delay between changes
  }
}