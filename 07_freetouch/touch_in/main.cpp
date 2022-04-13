/*
 * Seeeduino XIAO - testing Ax pin that can be capacitive touch inputs
 */

#include <Arduino.h>
#include "Adafruit_FreeTouch.h"

#define NUM_TOUCH_PINS 7

// create an instance of Adafruit_FreeTouch for each touch pin of the XIAO

Adafruit_FreeTouch qt[NUM_TOUCH_PINS] = {
  Adafruit_FreeTouch( A0, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE ),
  Adafruit_FreeTouch( A1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE ),
  Adafruit_FreeTouch( A6, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE ),
  Adafruit_FreeTouch( A7, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE ),
  Adafruit_FreeTouch( A8, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE ),
  Adafruit_FreeTouch( A9, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE ),
  Adafruit_FreeTouch( A10, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE ),
};

// Maximum normal capacitance measures of each touch pin
int threshold[NUM_TOUCH_PINS] { 0, 0, 0, 0, 0, 0, 0};

// Update the maximum capacitance measure during calibration
void updateThreshold(int touchPin, int val) {
  if (val > threshold[touchPin]) threshold[touchPin] = val;
}

// Set the threshold at scale * maximum recorded capacitance measure
void setThresholds(float scale) {
  for (int touchPin=0; touchPin < NUM_TOUCH_PINS; touchPin++) {
    threshold[touchPin] = (int) (scale*threshold[touchPin]);
  }
}

// Measure the capacitance of each pin count times to find the
// maximum value and then scale it to establish the threshold.
void calibrateThresholds(int count, float scale) {
  for (int n=0; n<count; n++) {
    for (int touchPin=0; touchPin < NUM_TOUCH_PINS; touchPin++) {
      updateThreshold(touchPin, qt[touchPin].measure());
      delay(5);
    }
  }
  setThresholds(scale);
}

// Return 1 if val > threshold or 0 otherwise
int touched(int touchPin, int val) {
  if (val > threshold[touchPin])
    return 1;
  return 0;
}

// flag if the setup succeeded
bool allgood = true;

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection 
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  // initialize the touch pins
  for (int touchPin = 0; touchPin < NUM_TOUCH_PINS; touchPin++ ) {
    if (! qt[touchPin].begin()) {
      Serial.printf("Failed to initialize Touch class on touch pin %d\n", touchPin);
      allgood = false;
    }
  }
  if (allgood) {
    calibrateThresholds(200, 1.1);

#ifdef PLATFORMIO
    Serial.println("thresholds:");
    for (int touchPin = 0; touchPin < NUM_TOUCH_PINS; touchPin++ ) {
      Serial.printf("  touch pin %d: %d\n", touchPin, threshold[touchPin]);
    }
#endif
  }
#ifdef PLATFORMIO
  Serial.println("setup() completed.");
#endif
}

// continuously read the first two touch pins and print their touched value.
void loop() {
  if (allgood) {
    // run test on first two touch pins.
    int val0 = touched(0, qt[0].measure());
    delay(5); // needed!
    int val1 = touched(1, qt[1].measure());
#ifdef PLATFORMIO
    if (val0) Serial.println("Touch pin 0 touched");
    if (val1) Serial.println("Touch pin 1 touched");
#else
    Serial.printf("%d\t%d\n", val0, val1);
#endif
    delay(20);
  } else {
    Serial.println("Setup failed");
    delay(4000);
  }
}
