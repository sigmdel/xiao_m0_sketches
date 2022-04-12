#include <Arduino.h>

int setPwm(int pin, int duty) {
  if (duty >= 255) {
    duty = 255;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  } else if (duty <= 0) {
    duty = 0;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  } else  {
    analogWrite(pin, duty);
  }
  //Serial.println(duty);
  return duty;
}


void setup() {
  // Time to upload new firmware
  delay(8000);
  // Wait for a USB connection
  Serial.begin(0);
  while (!Serial) delay(10);
  Serial.println("\nStarting setup()");

  for (int i = 0; i<14; i++) {
    pinMode(i, OUTPUT);
    Serial.printf("Pin %s%d mode set to output\n", (i<11) ? "A" : "", i);
  }

  Serial.println("setup() completed, starting loop()");
}

int pin = 1;

void loop() {
  Serial.printf("Fading pin %d", pin);
  if (pin)
    Serial.println();
  else
    Serial.println(" will not work");
  if (pin > 10)
    delay(500);  // minimize USB activity LED flashes
  else
    delay(100);

  if (pin < 11) {
    for (int i = 0; i < 17; i++) {
      setPwm(pin, i*16);
      delay(100);
    }
    for (int i = 15; i >= 0; i--) {
      setPwm(pin, i*16);
      delay(100);
    }
  } else {
    for (int i = 16; i >= 0; i--) {
      setPwm(pin, i*16);
      delay(100);
    }
    for (int i = 1; i < 17; i++) {
      setPwm(pin, i*16);
      delay(100);
    }
  }
  pin = (++pin)%14;
}
