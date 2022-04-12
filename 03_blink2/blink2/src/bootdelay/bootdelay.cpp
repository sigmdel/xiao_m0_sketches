#include <Arduino.h>
#include "bootdelay.h"

#define LED_OFF  HIGH  // this is a XIAO'sm

// times in milliseconds
void startDelay(uint16_t flashperiod, uint32_t timeout, uint32_t mindelay) {
  SerialUSB.begin(9600);  // baud is ignored
  bool usbConnected = false;
  uint16_t period = (!flashperiod || flashperiod < 100) ? 100 : flashperiod;
  unsigned long ms = millis();
  while (true) {
    if (SerialUSB && !usbConnected) {
      SerialUSB.println();
      usbConnected = true;
    }
    if ((millis() > mindelay) && usbConnected)
      return;
    if ((millis() > timeout) && (timeout > 0))
      return;
    if (flashperiod)
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle LED state
    delay(period);
  }
}

// times in seconds
bool startupDelay(uint16_t flashperiod, uint16_t timeout, uint16_t mindelay) {
  if (flashperiod) {
    pinMode(LED_BUILTIN, OUTPUT);
  }
  if (timeout) {
    timeout = (timeout > mindelay) ? timeout : mindelay;
  }
  startDelay(flashperiod, timeout*1000, mindelay*1000);
  if (flashperiod) {
    digitalWrite(LED_BUILTIN, LED_OFF);  // ensure the LED is off,
    pinMode(LED_BUILTIN, INPUT);      // restore the original pin mode
  }
  return (SerialUSB) ? true : false;
}
