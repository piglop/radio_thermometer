#include <OneWire.h>
#include <VirtualWire.h>

// DS18S20 Temperature chip i/o
OneWire ds(10);  // on pin 10

void setup(void) {
  // initialize inputs/outputs
  // start serial port
  Serial.begin(115200);
  pinMode(13, OUTPUT);     

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
}

char * floatToString(char * outstr, double val, byte precision, byte widthp){
  char temp[16];
  byte i;

  // compute the rounding factor and fractional multiplier
  double roundingFactor = 0.5;
  unsigned long mult = 1;
  for (i = 0; i < precision; i++)
  {
    roundingFactor /= 10.0;
    mult *= 10;
  }

  temp[0]='\0';
  outstr[0]='\0';

  if(val < 0.0){
    strcpy(outstr,"-\0");
    val = -val;
  }

  val += roundingFactor;

  strcat(outstr, itoa(int(val),temp,10));  //prints the int part
  if( precision > 0) {
    strcat(outstr, ".\0"); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
      mult *=10;

    if(val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val)- val ) * mult;
    unsigned long frac1 = frac;

    while(frac1 /= 10)
      padding--;

    while(padding--)
      strcat(outstr,"0\0");

    strcat(outstr,itoa(frac,temp,10));
  }

  // generate space padding 
  if ((widthp != 0)&&(widthp >= strlen(outstr))){
    byte J=0;
    J = widthp - strlen(outstr);

    for (i=0; i< J; i++) {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp,outstr);
    strcpy(outstr,temp);
  }

  return outstr;
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];


  if ( !ds.search(addr)) {
//      Serial.print("No more addresses.\n");
      ds.reset_search();
      delay(1000);
      return;
  }
  /*
  Serial.print("R=");
  for( i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }
  */
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }
/*
  if ( addr[0] == 0x10) {
      Serial.print("Device is a DS18S20 family device.\n");
  }
  else if ( addr[0] == 0x28) {
      Serial.print("Device is a DS18B20 family device.\n");
  }
  else {
      Serial.print("Device family is not recognized: 0x");
      Serial.println(addr[0],HEX);
      return;
  }
*/
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
/*
  Serial.print("P=");
  Serial.print(present,HEX);
  Serial.print(" ");
*/
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  //  Serial.print(data[i], HEX);
  //  Serial.print(" ");
  }
/*
  Serial.print(" CRC=");
  Serial.print( OneWire::crc8( data, 8), HEX);
  Serial.println();
*/
  int low = data[0];
  int high = data[1];
  /*
  float temp = (float)((high << 8) | low) / 16.0;

  Serial.print("TEMP=");
  Serial.print(temp);
  Serial.println();
  */

  unsigned char msg[16];
  int len = 0;

  msg[0] = 'T';
  len = 1;
  for(i = 0; i < 8; i++) {
    msg[len++] = addr[i];
  }
  for(i = 0; i < 2; i++) {
    msg[len++] = data[i];
  }
  
  /*
  Serial.print("MSG=");
  for( i = 0; i < len; i++) {
    Serial.print(msg[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  */


  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  vw_send((uint8_t *)msg, len);
  vw_wait_tx(); // Wait until the whole message is gone
  delay(100);
  digitalWrite(13, LOW);   // turn the LED on (HIGH is the voltage level)
}
