/*
  basicwdt

  test of the basic watchdog by Adrian Adamski
  <https://github.com/ajadamsky/Watchdog-Arudino-MKR-WiFi-1010>
*/

#include <Arduino.h>
#include "src/samdwdt/samdwdt.h"

unsigned long wdtTimer;

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  pinMode(LED_BUILTIN, OUTPUT);
  wdtTimer = millis();
  wdtFeed();
  Serial.println("Watchdog initiated");
  Serial.println("setup() completed, starting loop()");
}


int count = 5;
int timecount = 1;

void loop() {
  if ((millis() - wdtTimer > 7000) && count) {
    Serial.printf("Feeding watchdog after 7 seconds - countdown: %d\n", count);
    wdtFeed();
    wdtTimer = millis();
    count--;
    if (!count)
      Serial.println("No longer feeding watchdog");
  }
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(100);

  if (!count) {
    Serial.printf("%d ms\n", timecount*100);
    timecount++;
  }
}
