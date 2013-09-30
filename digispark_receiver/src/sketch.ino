#include <DigiUSB.h>
#include <VirtualWire.h>

void setup()
{
  
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);    // Bits per sec
  vw_set_rx_pin(1);
  vw_rx_start();     // Start the receiver PLL running
  
  
  DigiUSB.begin();
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A   
}

void loop()
{
  digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, HIGH);
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  //if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    int i;
    DigiUSB.refresh();
    DigiUSB.println("Hello");
/*    for( i = 0; i < buflen; i++) {
        DigiUSB.print(buf[i], HEX);
        DigiUSB.print(" ");
    }
    DigiUSB.println("");*/
  }
  
  digitalWrite(0, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(1, LOW); 
  DigiUSB.delay(1000);
}
