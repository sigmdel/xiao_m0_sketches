/*
 * Seeeduino XIAO - testing all Ax pins as digital inputs
 *
 *
 * Testing that every pin A0 to A10 can
 * be used as a digital input
 *
 * When INPUT_MODE = INPUT_PULLUP
 *   Use a small resistor connected to ground as a probe to
 *   test that each I/0 pin of the XIAO can be read as LOW
 *
 *   +-------[ 220R ]------> o== Ax
 *   |
 *   ⏚
 *
 * When INPUT_MODE = INPUT_PULLDOWN
 *   Use a small resistor connected to 3.3V as a probe to
 *   test that each I/0 pin of the XIAO can be read as HIGH
 *
 *   △ 3.3V
 *   |
 *   |
 *   +-------[ 220R ]------> o== Ax
 *
 */

#include <Arduino.h>

#define INPUT_MODE INPUT_PULLDOWN  // INPUT_PULLUP or INPUT_PULLDOWN

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  // initialize I/O signals connected to the LED
  for (int pin=0; pin<11; pin++) {
    pinMode(pin, INPUT_MODE);
    Serial.printf("Initialized pin A%d as input with internal %s resistor.\n", pin, (INPUT_MODE == INPUT_PULLUP) ? "pullup" : "pulldown");
  }

  pinMode(LED_BUILTIN, OUTPUT);
  for (int i=0; i<8; i++) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(75);
  }
  digitalWrite(LED_BUILTIN, HIGH); // turn LED off
  Serial.println("setup() completed, starting loop(), start probing");
}

int pin = 0;
int prevpin = -1;
int prevlevel =  -1;

void loop() {
  int level = digitalRead(pin);

  if (pin == prevpin) {
    if (level != prevlevel)  {
      Serial.printf("Pin A%d is %s\n", pin, (level) ? "HIGH" : "LOW");
      prevlevel = level;
    }

  } else if ((INPUT_MODE == INPUT_PULLUP) && !level) {
    Serial.printf("Pin A%d is LOW\n", pin);
    prevpin = pin;
    prevlevel = level;

  } else if ((INPUT_MODE == INPUT_PULLDOWN) && level) {
    Serial.printf("Pin A%d is HIGH\n", pin);
    prevpin = pin;
    prevlevel = level;
  }
  pin = (++pin)%11;
}
