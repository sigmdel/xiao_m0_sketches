/*
  blink2

  Blink for Seeeduino XIAO

  Flashes the ellow (user) LED and the two blue (USB activity) LEDS in sequence

  There are four LEDs on the XIAO, arranged in a square pattern,
    Top row:
      (P) Green power LED (always on when the board is powered up),
      (L) Yellow "free" or "user" LED to the right of the power led
    Bottom row:
      (R) Blue USB RX activity LED on the left,
      (T) Blue USB TX activity LED on the right.

  +---------------------+
  | o  o  o  o  o  o  o |
  |+------------+       |
  ||            |  o  o |   <--- Ground  Reset pads
  ||            |+--------+
  ||            ||  USB   |
  ||            ||   C    |
  ||            |+--------+
  ||            | P= =L |   <--- Green Power   Yellow Led
  |+------------+ R= =T |   <--- Blue  Rx      Blue   Tx
  | o  o  o  o  o  o  o |
  +---------------------+

  The L, R and T leds can be controlled with software. They are active
  LOW (writing 0 to GPIO pin connected to the LED turns it on).

  This example code is in the public domain.

  Michel Deslierres
  March 19, 2020
  March 30, 2022
*/

#include <Arduino.h>
#include "src/bootdelay/bootdelay.h"


// Change these values to confirm that a new version of the firmware has been uploaded
#define FLASHES         6
#define SHORT_DELAY   100 // 25 ms or more to perceive the individual flashes
#define LONG_DELAY    900

int LEDS[3] = {LED_BUILTIN, PIN_LED_TXL, PIN_LED_RXL};
const char* Names[3] = {"yellow LED (L)", "blue LED (T)X", "blue LED (R)X"};

int ledIndex;

void setup() {
  Serial.println("This will never be seen");

  // Wait up to 8 seconds, flashing the yellow LED
  startupDelay(200);
  // These Serial.print() will be seen if the serial monitor is opened during the
  // startup delay. They will not cause a problem if the serial monitor is
  // never opened. The rest of the code is executed no matter what.
  Serial.println("Starting blink2\n---------------");

  // initialize I/O signals connected to the LED
  for (ledIndex=0; ledIndex<3; ledIndex++) {
    pinMode(LEDS[ledIndex], OUTPUT);
    digitalWrite(LEDS[ledIndex], HIGH); // turn the LED off, should not be necessary
    Serial.printf("Initialized %s.\n", Names[ledIndex]);
  }

  // start with blue LED
  ledIndex = 1;
  Serial.println("setup() completed, starting loop()");
}


void loop() {
  pinMode(PIN_LED_TXL, INPUT);           // turn off USB activity LED
  Serial.printf("Flashing %s", Names[ledIndex]);
  digitalWrite(PIN_LED_TXL, HIGH);
  pinMode(PIN_LED_TXL, OUTPUT);          // restore USB activity LED

  for (int i=0; i<FLASHES; i++) {
    digitalWrite(LEDS[ledIndex], LOW);   // turn a LED on
    delay(SHORT_DELAY);                  // wait
    digitalWrite(LEDS[ledIndex], HIGH);  // turn the LED off
    delay(SHORT_DELAY);                  // wait
  }

  pinMode(PIN_LED_TXL, INPUT);           // turn off USB activity LED
  Serial.println(".");
  digitalWrite(PIN_LED_TXL, HIGH);
  pinMode(PIN_LED_TXL, OUTPUT);          // restore USB activity LED

  delay(LONG_DELAY);
  ledIndex = (ledIndex + 1)%3;            // next LED
}
