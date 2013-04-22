// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
#include <VirtualWire.h>
void setup()
{
  Serial.begin(115200); // Debugging only
  Serial.println("setup");
  pinMode(11, INPUT);
  pinMode(13, OUTPUT);
  // Initialise the IO and ISR
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
    buf[buflen] = 0;
    Serial.print((char*)buf);
    Serial.println("");
  }
  
  if (led_start && millis() > led_start + 100) {
    led_start = 0;
    digitalWrite(13, LOW);
  }
}
