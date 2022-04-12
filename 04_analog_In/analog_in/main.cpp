/*
 * Seeeduino XIAO - looking at the analog input on all Ax pins
 *
 * Use two resistor to create a voltage divider across the
 * Vcc (assumed to be 3.3 volt) and ground rails to test each pin.
 *
 *   △ Vcc (<= 3.3V)
 *   |
 *  +-+
 *  | |
 *  | | R1
 *  +-+
 *   |
 *   +-----------> Vout = Vcc*R2/(R1+R2)
 *   |
 *  +-+
 *  | | R2
 *  | |
 *  +-+
 *   |
 *   ⏚
 *
 * If R1 = R2 then Vout = Vcc/2
 *
 * Measured 1.657v with Vcc = 3.3 V and R1 = R2 = 2.2K nominally.
 *
 */

// Define if the analog read offset and gain adjustemnt values have been calculated.
// Use the CorrectADCResponse sketch to calculate these.
// In principle, reading 0 volts should yeat a 0 value and reading Vcc should yild a 4095 value.
//#define ANALOG_READ_CORRECTION

#ifdef ANALOG_READ_CORRECTION
  // Set the calculated analog read offset and gain values
  #define ANALOG_OFFSET    0  // 0 - no correction
  #define ANALOG_GAIN   2067  // 2048 - no correction
#endif

#include <Arduino.h>
#ifdef ANALOG_READ_CORRECTION
  #include "src/SAMD_AnalogCorrection/src/SAMD_AnalogCorrection.h"
#endif

int pin = A2;    // pin to test

  // The default ADC resolution is 10 bits set in
  // .../packages/framework-arduino-samd-seeed/cores/arduino/wiring_analog.c
  // The highest hardware ADC resolution 12 bits,
  //    #define ADC_RESOLUTION 12 is in .../XIAO_m0/variant.h
  // But the logical ADC resolution can be set arbitrarily, try 24
#define ADC_BITS 12

uint32_t maxADC = (1<<ADC_BITS)-1;
uint32_t lowLevel = (uint32_t) maxADC/3;
uint32_t highLevel = maxADC - lowLevel;

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection 
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  analogReadResolution(ADC_BITS);
  Serial.printf("Set the analog read resolution to %d bits.\n", ADC_BITS);
  Serial.printf("analogRead() returned values will be between 0 and %u inclusively.\n", maxADC);
  Serial.printf("Any value above %u will be interpreted as a connection to Vcc.\n", highLevel);
  Serial.printf("Any value below %u will be interpreted as a connection to ground.\n", lowLevel);
  Serial.println("Values in between will be interpreted as a connection to the voltage divider.");

  pinMode(pin, INPUT);
  Serial.printf("Initialized pin A%d as an input.\n", pin);

#ifdef ANALOG_READ_CORRECTION
  analogReadCorrection(ANALOG_OFFSET, ANALOG_GAIN);
  Serial.printf("Set analogReadCorrection, offset = %d, gain = %d\n", ANALOG_OFFSET, ANALOG_GAIN);
#endif

  Serial.println("setup() completed, starting loop()");
}

#define MODE_GND      0
#define MODE_VIN      1
#define MODE_VCC      2

int mode = MODE_VIN;
unsigned long sum = 0;
unsigned long count = 0;

void loop() {
  uint32_t level = analogRead(pin);
  if ( (level > highLevel) && (mode != MODE_VCC) ) {
    count = 0;
    sum = 0;
    mode = MODE_VCC;
    Serial.println("\nConnected pin to 3.3V, resetting average");

  } else if ( (level < lowLevel) && (mode != MODE_GND) ) {
    count = 0;
    sum = 0;
    Serial.println("\nConnected pin to 0V, resetting average");
    mode = MODE_GND;

  } else if ( (level >= lowLevel) && (level <= highLevel) && (mode != MODE_VIN) ) {
    count = 0;
    sum = 0;
    Serial.println("\nConnected pin to voltage divider, resetting average");
    mode = MODE_VIN;

  } else {
    // mode unchanged, accumulate values to calculate the average
    sum += level;
    count ++;
    if (count % 10 == 0) {
      double avg = (sum*1.0)/(count*1.0);
      Serial.printf("Count: %d, current level: %d, average: %.1f\n", count, level, avg);
    }
  }
  delay(250);
}
