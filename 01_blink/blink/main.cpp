#include <Arduino.h> 

// Use an assymmetric delay to confirm that the LEDs are active low.
// Change the delay values to confirm that a new version of the firmware has been uploaded
#define SHORT_DELAY    25 // 1/40 second on
#define LONG_DELAY   1000 // 1 second off

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // yellow LED that will be flashed
  pinMode(PIN_LED_TXL, INPUT);   // turn off the USB activity
  pinMode(PIN_LED_RXL, INPUT);   // LEDS to better see the yellow LED
}

int n = 0;

void loop() {
  n++;                              // increment the loop counter
  digitalWrite(LED_BUILTIN, LOW);   // turn a LED on
  Serial.printf("%d: ON... ", n);   // say so on the serial monitor
  delay(SHORT_DELAY);               // wait
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off
  Serial.println("off.");           // say it is off and terminate the line
  delay(LONG_DELAY);                // wait
}                                   // repeat the loop
