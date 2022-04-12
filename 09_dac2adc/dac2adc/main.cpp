/*
 dac2adc

 Analog reading of analog output on Seeeduino XIAO

 Wire together A0 and A2 (or any XIAO I/O pin other than A0).

 In the Arduino IDE, run and open Tools/Serial Plotter to
 see both the input and output values.
*/

// select the ADC input pin, could be any pin from A1 to A10
#define ADC_IN   A2

// select the scaling to use
#define RAW_DATA   0  // show raw numeric values written to DAC and read from ADC
#define SCALE_DAC  1  // scale the DAC output  by 4 (2^12 = 4 * 2^10)
#define VOLTS      2  // scale the DAC output and ADC input to show volts (float)
#define SCALE      SCALE_DAC

// Resolutions
//
#define DAC_BITS  8  // not used except in analogWriteResolution(DAC_BITS) where it has no effect;

#define ADC_BITS  11 // max is ADC_RESOLUTION; no scaling if ADC_BITS < 10
   //#define ADC_RESOLUTION 12 in .../XIAO_m0/variant.h

// Define if the analog read offset and gain adjustemnt values have been calculated.
// Use the CorrectADCResponse sketch to calculate these and set the ANALOG_OFFSET
// and ANALOG_GAIN values accordingly. With this correction reading 0 and
// Vcc = 3.3 volts should correspond to a 0 and 4095 values respectively when the
// ADC resolution is 12 bits (in principle).
//
//#define ANALOG_READ_CORRECTION

#ifdef ANALOG_READ_CORRECTION
  // Set the calculated analog read offset and gain values
  #define ANALOG_OFFSET    0  // 0 - no correction
  #define ANALOG_GAIN   2067  // 2048 - no correction
#endif

// --- no user settable values below ---

#include <Arduino.h>
#ifdef ANALOG_READ_CORRECTION
  #include "src/SAMD_AnalogCorrection/src/SAMD_AnalogCorrection.h"
#endif

// Connect DAC0 (A0) output to ADC_IN pin as defined above
#define DAC_OUT DAC0  // DAC output pin - A0
#define TRUE_DAC_BITS 10

const int maxDACoutputValue = (1<<TRUE_DAC_BITS)-1;
const int maxADCinputValue = (1<<ADC_BITS)-1;

#if (SCALE == VOLTS)
  #define VREF 3.3
  const float dacToVolt =  VREF/maxDACoutputValue;
  const float adcToVolt =  VREF/maxADCinputValue;
#elif (SCALE == SCALE_DAC)
  #if (ADC_BITS >= TRUE_DAC_BITS)
  int scaleFactor = 1<<(ADC_BITS - TRUE_DAC_BITS);
  #else
  int scaleFactor = 1;  // no scaling
  #endif
#endif


int step = 16;

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection 
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  //pinMode(DAC_OUT, OUTPUT);     // ***** DO NOT set DAC0 = A0 mode to OUTPUT when using the DAC *****
  pinMode(ADC_IN, INPUT);
  Serial.printf("Initialized pin A%d as an input.\n", ADC_IN);

  analogWriteResolution(DAC_BITS);
  Serial.printf("Set DAC resolution of output pin %d to %d bits.\n", DAC_OUT, DAC_BITS);
  #if (DAC_BITS != TRUE_DAC_BITS)
  Serial.println("DAC resolution is 10 bits, no matter the value set.");
  #endif
  Serial.println("The analogWriteResolution() function should not be used in this context.");
  Serial.printf("Output DAC values range from 0 to %d/\n", maxDACoutputValue);

  analogReadResolution(ADC_BITS);
  Serial.printf("Set ADC resolution of input pin %d to %d bits.\n", ADC_IN, ADC_BITS);
  Serial.printf("Input ADC values range from 0 to %d\n", maxADCinputValue);

#ifdef ANALOG_READ_CORRECTION
  analogReadCorrection(ANALOG_OFFSET, ANALOG_GAIN);
  Serial.printf("Set analogReadCorrection, offset = %d, gain = %d\n", ANALOG_OFFSET, ANALOG_GAIN);
#endif

#if (SCALE == SCALE_DAC)
  Serial.printf("Output DAC values scaled up by %d to match input ADC values\n", scaleFactor);
#elif (SCALE == VOLTS)
  Serial.println("Output DAC values and input ADC values converted to volts");
#else
  Serial.println("Raw output DAC and input ADC values shown.");
#endif

  Serial.println("setup() completed, starting loop()");
}

int invalue;
int outvalue = 0;

void loop() {
    analogWrite(DAC_OUT, outvalue);
    delay(2); // time to settle, probably not necessary
    invalue = analogRead(ADC_IN);
#ifdef PLATFORMIO
    #if (SCALE == SCALE_DAC)
      Serial.printf("OUT(DAC):%d\tIN(ADC):%d\n", scaleFactor*outvalue, invalue);
    #elif (SCALE == VOLTS)
      Serial.printf("OUT(V):%.4f\tIN(V):%.4f\n", dacToVolt*outvalue, adcToVolt*invalue);
    #else // (SCALE == RAW_DATA)
      Serial.printf("OUT(DAC):%d\tIN(ADC):%d\n", outvalue, invalue);
    #endif
#else // Arduino
    #if (SCALE == SCALE_DAC)
      Serial.printf("OUT(DAC):%d\tIN(ADC):%d\tMax:4095\n", scaleFactor*outvalue, invalue);
    #elif (SCALE == VOLTS)
      Serial.printf("OUT(V):%.4f\tIN(V):%.4f\n", dacToVolt*outvalue, adcToVolt*invalue);
    #else // (SCALE == RAW_DATA)
      Serial.printf("OUT(DAC):%d\tIN(ADC):%d\tMaxDAC:1023\tMaxADC:4095\n", outvalue, invalue);
    #endif
#endif
    outvalue += step;
    if ((outvalue < 0) || (outvalue > maxDACoutputValue)) {
      step = -step;
      if (outvalue < 0)
        outvalue = 0;
      else
        outvalue = maxDACoutputValue;
    }
    delay(10);
}
