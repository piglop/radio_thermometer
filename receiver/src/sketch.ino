#include <VirtualWire.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x12
int dataReceived = 0;

#define OUTPUT_BUFFER_SIZE 256
unsigned char output_buffer[OUTPUT_BUFFER_SIZE];
int output_buffer_start = 0;
int output_buffer_length = 0;

void send_data()
{
  if (output_buffer_length > 0) {
    Wire.write(output_buffer[output_buffer_start]);
    output_buffer_start++;
    if (output_buffer_start >= OUTPUT_BUFFER_SIZE)
      output_buffer_start = 0;
    output_buffer_length--;
  }
}

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

  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(send_data);
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
        output_buffer[(output_buffer_start + output_buffer_length) % OUTPUT_BUFFER_SIZE] = buf[i];
        output_buffer_length++;
    }
    Serial.println("");
  }
  
  if (led_start && millis() > led_start + 100) {
    led_start = 0;
    digitalWrite(13, LOW);
  }
}
