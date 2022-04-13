/*
  ldr_in
*/

// Define if a text output to the serial monitor is desired
#define PLOT

// Define to size of the FIFO queue to calculate a rolling average of LDR values.
// if less than 2, no averaging is done
#define LDR_FIFO_SIZE    10

// Define if the analog read offset and gain adjustemnt values have been calculated.
// Use the CorrectADCResponse sketch to calculate these.
// In principle, reading 0 volts should yeat a 0 value and reading Vcc should yild a 4095 value.
#define ANALOG_READ_CORRECTION

#ifdef PLATFORMIO
#undef PLOT
#endif

#ifdef ANALOG_READ_CORRECTION
  // Set the calculated analog read offset and gain values
  #define ANALOG_OFFSET    0  // 0 - no correction
  #define ANALOG_GAIN   2067  // 2048 - no correction
#endif

#include <Arduino.h>
#ifdef ANALOG_READ_CORRECTION
  #include "src/SAMD_AnalogCorrection/src/SAMD_AnalogCorrection.h"
#endif


#if (LDR_FIFO_SIZE > 1)

// variables used for rolling average LDR value
int ldrValues[LDR_FIFO_SIZE] {0};
int ldrIndex = 0;
int ldrCount = 0;
int ldrSum = 0;

// adds the a new LDR value and returns the new average
int addLdrValue(int newValue) {
     ldrSum = ldrSum - ldrValues[ldrIndex] + newValue;
     ldrValues[ldrIndex] = newValue;
     if (ldrCount < LDR_FIFO_SIZE) ldrCount++;
     ldrIndex = (ldrIndex + 1) % LDR_FIFO_SIZE;
     return (int) ldrSum / ldrCount;
}

#endif

// Analog input pin
#define LDR 3

void setup() {
  // Time to upload new firmware
  delay(8000);
#ifndef PLOT
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup().");
#endif

  pinMode(LDR, INPUT);
#ifndef PLOT
  Serial.printf("LDR connected to pin A%d.\n", LDR);
#endif

  analogReadResolution(ADC_RESOLUTION);  // #defined in ../XIAO_m0/variant.h
#ifndef PLOT
  Serial.printf("The ADC resolution set to %d bits\n", ADC_RESOLUTION);
  Serial.printf("Returned LDR values will be in the range 0-%d.\n", (1<<ADC_RESOLUTION)-1);
#endif

#ifdef ANALOG_READ_CORRECTION
  analogReadCorrection(ANALOG_OFFSET, ANALOG_GAIN);
#ifndef PLOT
  Serial.printf("Set analogReadCorrection, offset = %d, gain = %d\n", ANALOG_OFFSET, ANALOG_GAIN);
#endif  
#endif

#ifndef PLOT
#if (LDR_FIFO_SIZE < 2)
  Serial.println("Only raw LDR data is returned.");
#else
  Serial.println("A rolling average of the last %d LDR data and the raw LDR data is returned.");
#endif

  Serial.println("setup() completed, starting loop().");
#endif  
}

void loop(){
  int level = analogRead(LDR);

#if (LDR_FIFO_SIZE < 2)                    // return raw LDR values

#ifdef PLOT
  Serial.println(level);                   // no text is better for the serial plotter
#else
  Serial.printf("LDR level: %d\n", level); // ok for the serial monitor, but
#endif

#else                                      // return raw and average LDR value

  int avg = addLdrValue(level);

#ifdef PLOT
  Serial.printf("%d\t%d\n", level, avg);                     // no text is better for the serial plotter
#else
  Serial.printf("LDR level: %d, average: %d\n", level, avg); // ok for the serial monitor, but
#endif

#endif

  delay(2000);
}
