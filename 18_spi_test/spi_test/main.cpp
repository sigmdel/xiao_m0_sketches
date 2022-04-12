/*
  spi_test

  SPI loopback test

*/

#include <SPI.h>

const int CS = A3;   // SS = A4 = 4 but that would mean loosing I²C port

void setup (void) {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup().");

  Serial.println("Initializing SPI port");
  Serial.printf("  MOSI (output pin): %d\n", MOSI);
  Serial.printf("  MISO (input pin):  %d\n", MISO);
  Serial.printf("  SCK  (clock):      %d\n", SCK);
  Serial.printf("  CS   (select):     %d\n", CS);

  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);                // disable Slave Select
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);   //divide the clock by 8

  Serial.println("setup() completed, starting loop().");
}

#define BUFFER_SIZE  32

char outBuffer[BUFFER_SIZE] = {0};
char inBuffer[BUFFER_SIZE] = {0};
int count = 0;

void loop (void) {
  digitalWrite(CS, LOW); // enable Slave Select
  count++;
  int n = snprintf(outBuffer, BUFFER_SIZE, "Message %d", count);
  Serial.printf("\nTransmitting \"%s\"\n", outBuffer);

  for (int i = 0; i < n; i++) {
    inBuffer[i] = SPI.transfer(outBuffer[i]);
  }
  digitalWrite(CS, HIGH); // disable Slave Select

  Serial.print("Received: \"");
  for (int i=0; i < n; i++) {
    char c = inBuffer[i];
    if ((31 < c) && (c < 127))
      Serial.print(c);
    else
      Serial.printf(" 0x%.2x ", c);
  }
  Serial.println("\"");

  memset(inBuffer, 0, sizeof(inBuffer));
  delay(1000);
}
