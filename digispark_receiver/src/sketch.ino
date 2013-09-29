#include <DigiUSB.h>

void setup()
{
  DigiUSB.begin();
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A   
}

void loop()
{
  digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, HIGH);
  DigiUSB.refresh();
  DigiUSB.println("Hello");
  delay(250);
  digitalWrite(0, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(1, LOW); 
  delay(250);
}
