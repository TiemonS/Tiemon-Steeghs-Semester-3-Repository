#include <Arduino.h>
#include <SoftwareSerial.h>

#define RX_PIN 0
#define TX_PIN 1

SoftwareSerial mySerial(RX_PIN, TX_PIN); // software gebaseerde uarts port

// alle states waarin de machine zich kan bevinden
// IDLE
// SEND_IDENTIFICATION
// WAIT_FOR_ACK
// SEND_DATA
enum State {
  IDLE,
  SEND_IDENTIFICATION,
  WAIT_FOR_ACK,
  SEND_DATA
};

State currentState = IDLE; // de begin staat van de machine is idle

uint8_t StartBit = 1;
String Data = "data";
uint8_t StopBit = 0;

void setup() {
  mySerial.begin(9600);
}

void loop() {
  switch(currentState) {
    case IDLE:
      // do nothing and wait for start signal from PC
      if(mySerial.available() > 4 && mySerial.readString() == "Start") {
        // transition to SEND_HELLO state on start signal received
        currentState = SEND_IDENTIFICATION;
      }
      break;
      
    case SEND_IDENTIFICATION:
      // send a hello message to PC and wait for ACK
      mySerial.println("\nHello van het Redboard!");
      currentState = WAIT_FOR_ACK;
      break;
      
    case WAIT_FOR_ACK:
      // wait for ACK signal from PC
      if(mySerial.available() > 0 && mySerial.read() == 'A') {
        // transition to SEND_DATA state on ACK received
        currentState = SEND_DATA;
      }
      break;
      
    case SEND_DATA:
      // send a data message to PC and wait for response
      mySerial.println("");
      mySerial.print("Data from Arduino: ");
      mySerial.print(StartBit);
      mySerial.print(Data);
      mySerial.print(StopBit);
      mySerial.print('\n');
      currentState = IDLE; // transition back to IDLE state after data sent
      break;
  }
}