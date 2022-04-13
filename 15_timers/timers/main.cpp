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

#define USE_TIMER_TC3  // use TimerTc3, comment out to use TimerTcc0
#define TIMER_TIME 1000000   // micro seconds, careful too big a value will not work


#include <Arduino.h>

#ifdef USE_TIMER_TC3
  #include <TimerTC3.h>
  #define TIMER TimerTc3
#else  
  #define TIMER TimerTcc0
  #include <TimerTCC0.h>
#endif

volatile bool isLEDOn = false;

void timerIsr() {    
    digitalWrite(LED_BUILTIN, isLEDOn);
    isLEDOn = !isLEDOn;
}

void setup() {
    // Time to upload new firmware
    delay(8000);  
    // Do not wait for USB connection
    Serial.println("\nStarting setup()");
    
    pinMode(LED_BUILTIN, OUTPUT);    
    Serial.printf("LED_BUILTIN (A%d) set to OUTPUT mode\n", LED_BUILTIN);
    
    TIMER.initialize(TIMER_TIME);
    TIMER.attachInterrupt(timerIsr);
    Serial.printf("%d microseconds timer based blink using ", TIMER_TIME);
    
    #ifdef USE_TIMER_TC3
    Serial.println("TimerTc3");
    #else
    Serial.println("TimerTcc0");
    #endif  
    pinMode(PIN_LED_TXL, INPUT); // turn off serial tx LED signalling
}


bool oldState = !isLEDOn;

void loop() {
    int state;
    noInterrupts();
    state = isLEDOn;
    interrupts();  
    if (state != oldState) {
      Serial.print((state) ? "ON " : "OFF\n");
      oldState = state;
    }
}
