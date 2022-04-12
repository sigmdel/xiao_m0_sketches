/*
  uart

  test of the XIAO uart port
  <https://github.com/ajadamsky/Watchdog-Arudino-MKR-WiFi-1010>
*/

#include <Arduino.h>

#define SERIAL_BAUD  115200
#define WRITE_INTERVAL 3000  // transmit every 3 seconds

unsigned long writetimer;

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup().");

  Serial1.begin(SERIAL_BAUD);
  while (!Serial1) delay(10);
  Serial.printf("Hardware serial port (Serial1) initialized with %d baud.\n", SERIAL_BAUD);
  Serial.println("TX pin A6 must be connected to RX pin A7.");

  pinMode(PIN_LED_TXL, INPUT); // turn off USB activity LED 
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("setup() completed, starting loop().");
  writetimer = millis();
}

int count = 0;

void loop() {
  while (Serial1.available()) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.write(Serial1.read());
    delay(2); // otherwise the flashng LED is hardly visible
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (millis() - writetimer > WRITE_INTERVAL) {
    count++;
    Serial.printf("\nTransmitting \"Message %d\"\n", count);
    Serial1.printf("Message %d\n", count);
    writetimer = millis();
  }  
 }
