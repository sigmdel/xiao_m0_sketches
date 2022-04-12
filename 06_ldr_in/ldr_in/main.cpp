/*
  ldr_in
*/

// Define if a text output to the serial monitor is desired
//#define NO_PLOT

// Define if the analog read offset and gain adjustemnt values have been calculated.
// Use the CorrectADCResponse sketch to calculate these.
// In principle, reading 0 volts should yeat a 0 value and reading Vcc should yild a 4095 value.
#define ANALOG_READ_CORRECTION

#ifdef ANALOG_READ_CORRECTION
  // Set the calculated analog read offset and gain values
  #define ANALOG_OFFSET    0  // 0 - no correction
  #define ANALOG_GAIN   2067  // 2048 - no correction
#endif

#include <Arduino.h>
#ifdef ANALOG_READ_CORRECTION
  #include "src/SAMD_AnalogCorrection/src/SAMD_AnalogCorrection.h"
#endif

// Analog input pin
#define LDR 3

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection 
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  pinMode(LDR, INPUT);
  analogReadResolution(ADC_RESOLUTION);  // #defined in ../XIAO_m0/variant.h

#ifdef ANALOG_READ_CORRECTION
  analogReadCorrection(ANALOG_OFFSET, ANALOG_GAIN);
  Serial.printf("Set analogReadCorrection, offset = %d, gain = %d\n", ANALOG_OFFSET, ANALOG_GAIN);
#endif

  Serial.println("setup() completed, starting loop()");
}

int level;

void loop(){
  level = analogRead(LDR);
#if defined(PLATFORMIO) || defined(NO_PLOT)
  Serial.printf("LDR level: %d\n", level); // ok for the serial monitor, but
#else
  Serial.println(level);                   // no text is better for the serial plotter
#endif
  delay(2000);
}
