/*
 * dac2digin
 *
 * Determine the HIGH/LOW threhhold values of digital input pins
 *
 * Wire together A0 and Ax (where Ax is an I/O pin other than A0).
 *
 * In the Arduino IDE, run and open Tools/Serial Plotter to
 * see a graph of both the input and output values.
*/

#include <Arduino.h>

// Connect DAC output to any other XIAO pin
#define DAC_PIN A0  // DAC output pin
#define DIG_PIN A2  // digital input pin, could be any pin from A1 to A10

//#define PLOT      // show data in plotter monitor
#define DATA   0  // show raw numeric values written to DAC
#define VOLTS  1  // scale the DAC to volts

// select the scaling to use
#define SCALE  VOLT

// Resolution
//
#define DAC_RESOLUTION  10
   // 2^10 = 1024  (hence possible output value 0 to 1023)

#define VREF 3.3
const float dacToVolt =  VREF/((1<<DAC_RESOLUTION)-1);
const float digToVolt =  VREF;

#ifdef PLATFORMIO
#undef PLOT
#endif

#ifdef PLOT
int step = 16;
#else
int step = 1;
#endif

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");
  pinMode(DIG_PIN, INPUT);

  //pinMode(DAC_PIN, OUTPUT);     // ***** DO NOT set A0 mode to OUTPUT when using the DAC *****
  analogWriteResolution(DAC_RESOLUTION);
}

int invalue;
int outvalue = 0;
#ifndef PLOT
bool isLow = true;
#endif

void loop() {
    analogWrite(DAC_PIN, outvalue);
    delay(2);
    invalue = digitalRead(DIG_PIN);

#ifdef PLOT
    #if (SCALE == VOLTS)
      Serial.printf("OUT(V):%.4f\tIN(V):%.4f\n", dacToVolt*outvalue, digToVolt*invalue);
    #else // (SCALE == DATA)
      Serial.printf("OUT(DAC):%d\tIN(ADC*1023):%d\n", outvalue, invalue*1023);
    #endif
#else
    if (isLow && invalue) {
      Serial.printf("Transition from LOW to HIGH at %.4fV\n", dacToVolt*outvalue);
      isLow = false;
    } else if (!isLow && !invalue) {
      Serial.printf("Transition from HIGH to LOW at %.4fV\n", dacToVolt*outvalue);
      isLow = true;
    }
#endif
    outvalue += step;
    if ((outvalue < 0) || (outvalue > 1023)) {
      step = -step;
      if (outvalue < 0)
        outvalue = 0;
      else
        outvalue = 1023;
    }
    delay(10);
}
