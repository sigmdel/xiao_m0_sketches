/*

timers

Another blink but based on a timer interrupt service routine.

Based on two example sketches in the Examples for Seeeduino XAIO section:
  File/Examples/TimerTC3/ISRBlink, and File/Examples/TimerTC3/ISRBlink

See the TimerOne & TimerThree Libraries example program from PJRC
at https://www.pjrc.com/teensy/td_libs_TimerOne.html for an excellent
explanation of the use of the volatile type and the need to disable
interrupts in the main loop.

*/

#include <Arduino.h>

// select the timer to use
#define TIMER_TC3  0    // use a 16 bit resolution timer
#define TIMER_TCC0 1    // use a 24 bit resolution timer
#define USE_TIMER  TIMER_TC3

// set on and off times in microseconds
#define ON_TIME      750000  // Values in the 750000 to 1250000
#define OFF_TIME    1250000  // range work reliably with both timers


#if USE_TIMER == TIMER_TC3
  #include <TimerTC3.h>
  #define TIMER TimerTc3
#elif USE_TIMER == TIMER_TCC0
  #include <TimerTCC0.h>
  #define TIMER TimerTcc0
#else
  #ERROR "No timer defined"
#endif

unsigned long period[] {OFF_TIME, ON_TIME};

volatile int isLEDOn = 0;

void timerIsr() {
  digitalWrite(LED_BUILTIN, isLEDOn);
  isLEDOn = 1 - isLEDOn;
  TIMER.setPeriod(period[isLEDOn]);
}

unsigned long timer;

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Do not wait for a USB connection

  Serial.println("\nStarting setup().");
  Serial.printf("Using timer %s",
  #if USE_TIMER == TIMER_TC3
    "TimerTc3"
  #else
    "TimerTcc0"
  #endif
  );
  Serial.printf("\nOn/Off times in microseconds: %d, %d.\n", (int) period[true], (int) period[false]);
  Serial.printf("On/Off times in milliseconds: %d, %d.\n", (int) period[true]/1000, (int) period[false]/1000);

  pinMode(LED_BUILTIN, OUTPUT);    // initialize yellow user LED
  digitalWrite(LED_BUILTIN, HIGH); // turn off
  pinMode(PIN_LED_TXL, INPUT);     // turn off USB TX LED
  pinMode(PIN_LED_RXL, INPUT);     // turn off USB RX LED
  Serial.printf("Set LED_BUILTIN (%d) to OUTPUT and turned off USB LEDs.\n", LED_BUILTIN);

  TIMER.initialize(OFF_TIME);
  TIMER.attachInterrupt(timerIsr);

  timer = millis();
  Serial.println("setup() completed, starting loop().");
}

int oldState = isLEDOn;

void loop() {
  noInterrupts();
  int state = isLEDOn;
  interrupts();
  if (state != oldState) {
    Serial.printf("%.8u %s (was %s for: %u ms).\n", millis(), (state) ? "ON " : "OFF", (state) ? "off" : "on ", millis() - timer);
    oldState = state;
    timer = millis();
  }
}
