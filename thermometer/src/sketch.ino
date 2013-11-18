#include <OneWire.h>
#include <VirtualWire.h>

// DS18S20 Temperature chip i/o
OneWire ds(2);

void setup(void) {
  // initialize inputs/outputs
  // start serial port
  Serial.begin(115200);
  pinMode(0, OUTPUT);     
  pinMode(13, OUTPUT);     

  vw_set_tx_pin(5);
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  unsigned char msg[16];
  int len = 0;


  if ( !ds.search(addr)) {
      Serial.println("No sensor found");
      ds.reset_search();
      msg[0] = 'T';
      msg[1] = '-';
      len = 2;
      digitalWrite(13, HIGH);
      vw_send((uint8_t *)msg, len);
      vw_wait_tx();
      digitalWrite(13, LOW);
      delay(1000);
      return;
  }
  
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  delay(1000); // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
  
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  
  int low = data[0];
  int high = data[1];

  msg[0] = 'T';
  len = 1;
  for(i = 0; i < 8; i++) {
    msg[len++] = addr[i];
  }
  for(i = 0; i < 2; i++) {
    msg[len++] = data[i];
  }
  
  digitalWrite(13, HIGH);
  vw_send((uint8_t *)msg, len);
  vw_wait_tx();
  delay(100);
  digitalWrite(13, LOW);
}
