#include <Arduino.h>

char bericht[] = "Test";

void USART_Init(unsigned int ubrr)
{
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8); //voor de HIGH Byte moet je helemaal aan de rechter kant zijn dus 8 na rechts
UBRR0L = (unsigned char)ubrr; //De LOW Byte is volledig voor de baudrate dus kan worden gelijk gezet
//Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

unsigned long getMillis() 
{
  return millis();
}

void delayMillis(unsigned long delayTime) 
{
  unsigned long targetTime = getMillis() + delayTime;
  while (getMillis() < targetTime) {
    // niks doen gewoon de delay afwachten
  }
}

void setup() 
{
  long stap = 16UL * (long)9600;
  int ubrr = F_CPU / stap - 1;

  USART_Init(ubrr);
}

void PrintMessage(char Message[]) 
{
  for (size_t i = 0; i < strlen(Message); i++)
  {
    UDR0 = Message[i];  
    delayMillis(10);
  }  
}

void loop() 
{
  PrintMessage(bericht);
  delayMillis(500);
}

