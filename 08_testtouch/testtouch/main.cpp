/*
 * Seeeduino XIAO - testing Ax pin that can be capacitive touch inputs
 */

#include <Arduino.h>
#include "Adafruit_FreeTouch.h"

#define NUM_TOUCH_PINS 7  // A0, A1, A6 to A10
#define N 20              // Moving average parameter
#define scale 0.25        // "touched" or outlier proportion

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

// average capacitance level
int level[NUM_TOUCH_PINS] {0};

// Update the average capacitance measure.
// This is actually a weighted moving average with weights of older
// measures vanishing to 0
void updateLevel(int touchPin, int val) {
  level[touchPin] = (( N * level[touchPin] ) + val ) / ( N + 1 );
}

// Return 1 if the difference between the capacitance measure and the average level is big
// In other words return 1 if the new measure is an outlier with respect to the moving average
int touched(int touchPin, int measure) {
  bool big = ( abs(measure - level[touchPin]) > (int) (scale*level[touchPin]) );
  if (!big) updateLevel(touchPin, measure);  // dont include outliers in the moving average calculation
  return big;
}

void calibrateLevels(int count) {
  int n = (count < 3*N) ? 3*N : count;
  for (int i=0; i<n; i++) {
    for (int touchpin=0; touchpin<NUM_TOUCH_PINS; touchpin++) {
      updateLevel(touchpin, qt[touchpin].measure());
      delay(10);
    }
  }
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
    calibrateLevels(200);

#ifdef PLATFORMIO
    Serial.println("Moving average capacitance levels:");
    for (int touchPin = 0; touchPin < NUM_TOUCH_PINS; touchPin++ ) {
      Serial.printf("  touch pin %d: %d\n", touchPin, level[touchPin]);
    }
#endif
  }
#ifdef PLATFORMIO
  Serial.println("setup() completed.\n");
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
