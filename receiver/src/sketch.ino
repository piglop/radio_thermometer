#include <VirtualWire.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("setup");
  pinMode(11, INPUT);
  pinMode(13, OUTPUT);
  
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);    // Bits per sec
  vw_set_rx_pin(11);
  vw_rx_start();     // Start the receiver PLL running
}

unsigned long led_start = 0;

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    int i;
    digitalWrite(13, HIGH);
    led_start = millis();
    for( i = 0; i < buflen; i++) {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
    }
    Serial.println("");
  }
  
  if (led_start && millis() > led_start + 100) {
    led_start = 0;
    digitalWrite(13, LOW);
  }
}
