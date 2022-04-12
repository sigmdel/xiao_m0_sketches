/*
  interrupts

  Test interrupts on all Seeeduino XIAO pins.

  Interrupts are used to signal the state of an input pin. The yellow LED is on whenever an input
  pin is grounded.

  Pins A5 and A7 share interrupt 9. Both cannot be used at the same time.

  The distinct interrupts attached to pins A0 and A1 share a single interrupt service routine (ISR).
*/

#include <Arduino.h>

#define PIN_5_ONLY   0       // attach an interrupt to pin A5 but not to A7
#define PIN_7_ONLY   1       // attach an interrupt to pin A7 but not to A5
#define PIN_5_AND_7  2       // attach an interrupt to pins A5 and A7 - distinct ISRs   **** Neither pin can fire the interrupt ****
#define PIN_5_7_SAME_ISR 3   // attach an interrupt to pins A5 and A7 - same ISR        **** Neither pin can fire the interrupt ****

#define EXT_INT9_PIN  PIN_5_ONLY

#define DEBOUNCE_TIME 10 // millisecond button debouncing time

volatile int activeint = -1;
volatile bool reported = true;

void SR(int pin) {
  noInterrupts();
  delay(DEBOUNCE_TIME);
  digitalWrite(LED_BUILTIN, digitalRead(pin));
    /* Exploiting the fact that the XIAO LEDS are active LOW. Here is the logic:

      if (digitalRead(BUTTON) == HIGH) {
      // If the buttonState is HIGH, the button is release so turn the LED off:
      digitalWrite(LED_BUILTIN, HIGH);
      } else {
      // If the buttonState is LOW, the button is pressed so turn LED on:
      digitalWrite(LED_BUILTIN, LOW);
      }

    With an acive HIGH LED could use digitalWrite(LED_BUILTIN, !digitalRead(BUTTON));
    */
  activeint = pin;
  reported = false;
  interrupts();
}

// used for pins 0 and 1
void pin0_ISR() {
  SR(0);
}

void pin2_ISR() {
  SR(2);
}
void pin3_ISR() {
  SR(3);
}
void pin4_ISR() {
  SR(4);
}
void pin5_ISR() {
  SR(5);
}
void pin6_ISR() {
  SR(6);
}
void pin7_ISR() {
  SR(7);
}
void pin8_ISR() {
  SR(8);
}
void pin9_ISR() {
  SR(9);
}
void pin10_ISR() {
  SR(10);
}

void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  for (int pin=0; pin<11; pin++) {
    Serial.printf("DigitalPinToInterrupt(A%d) = %d, ", pin, digitalPinToInterrupt(pin));
    Serial.printf("  External interrupt: %d\n", g_APinDescription[pin].ulExtInt);
    // initialize the pin as an input and enable the internal pull up resistor:
    pinMode(pin, INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(0), pin0_ISR, CHANGE); // pin 0 and 1 interrupts
  attachInterrupt(digitalPinToInterrupt(1), pin0_ISR, CHANGE); // share an ISR
  attachInterrupt(digitalPinToInterrupt(2), pin2_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), pin3_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), pin4_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(6), pin6_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), pin8_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(9), pin9_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(10), pin10_ISR, CHANGE);

#if !(EXT_INT9_PIN == PIN_7_ONLY)
  attachInterrupt(digitalPinToInterrupt(5), pin5_ISR, CHANGE);
  Serial.println("button5_ISR attached to pin 5");
#endif

#if (EXT_INT9_PIN == PIN_5_7_SAME_ISR)
    attachInterrupt(digitalPinToInterrupt(7), pin5_ISR, CHANGE);
    Serial.println("button5_ISR attached to pin 7");
#elif !(EXT_INT9_PIN == PIN_5_ONLY)
    attachInterrupt(digitalPinToInterrupt(7), pin7_ISR, CHANGE);
    Serial.println("button7_ISR attached to pin 7");
#endif

  Serial.println("setup() completed, starting loop(), check each pin");
}

void loop() {
 // empty right now, add code as needed.
  if ( (!reported) && (activeint >= 0) ) {
    if (activeint)
      Serial.printf("State of pin A%d changed\n", activeint);
    else
      Serial.println("State of pin A0 or A1 changed");
    reported = true;
  }
}
