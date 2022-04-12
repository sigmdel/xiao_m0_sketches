/*
 * Seeeduino XIAO - testing all Ax pins as digital outputs
 *
 *
 * Testing that every pin A0 to A10 can
 * be used as a digital output
 *
 * Place a small resistor in series with a LED whose
 * cathode is connected to the ground and use the anode
 * as a probe to be connected to each I/0 pin of the
 * XIAO
 *
 *   +-----|◀----[ 220R ]------> o== Ax
 *   |    -  +
 *   ⏚
 *
 *  The LED should turn on when pin Ax is set HIGH
 *
 *  Even better, charlyplex two diodes as a probe
 *
 *   △ 3.3V
 *   |
 *   |     Top LED
 *   +------▶|---+
 *         +  -  |
 *               +---[22O R]-----> o== Ax
 *         -  +  |
 *   +------|◀---+
 *   |     Bottom LED
 *   ⏚
 *
 *  the bottom LED should turn on when pin Ax is set HIGH and off otherwise
 *  while the top LED should turn off only when pin Ax is set HIGH.
 */

#include <Arduino.h>      // needed for PlatformIO

#define DELAY 125

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection 
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  // initialize I/O signals connected to the LED
  for (int pin=0; pin<11; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // set the pin low
    Serial.printf("Initialized pin A%d as output.\n", pin);
  }

  Serial.println("setup() completed, starting loop()");
}

int pin = 0;

void loop() {
  Serial.printf("Pin A%d set to HIGH", pin);
  digitalWrite(pin, HIGH);
  delay(DELAY);
  digitalWrite(pin, LOW);
  Serial.println(", now LOW.");
  pin = (++pin)%11;
}
