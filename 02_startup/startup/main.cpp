#include <Arduino.h>

#define STARTUP_DELAY 8      // 8 seconds

unsigned long setupstart;    // time to reach setup() in microseconds
unsigned long startserial;   // time for USBSerial to come up in microseconds
unsigned long connecttimer;  // time for USB connection
bool serialstart;
bool serialbegin;
bool connected = false;      // Assume no USB connection established

void report(void) {
  Serial.println("\n--SAM D21 XIAO Startup Timing--");
  Serial.printf("setup() reached in %u microseconds (%.1f milliseconds).\n", setupstart, setupstart/1000.0);
  Serial.printf("(Serial) = %s %u microseconds (%.1f milliseconds) later.\n", (serialstart) ? "true" : "false", startserial, startserial/1000.0);
  Serial.printf("(Serial) = %s after Serial.begin(3214)\n", (serialbegin) ? "true" : "false");
  if (connected)
    Serial.printf("USB connected %u milliseconds (%.1f seconds) after USBSerial up. Baud: %d.\n", connecttimer, connecttimer/1000.0, Serial.baud());
  else
    Serial.printf("No USB connection made afer %d seconds.\n", STARTUP_DELAY);
}

void setup() {
  Serial.println("\nsetup() - 1");
  // Wait up to 10 seconds for Serial (= USBSerial) port to come up.
  // Usual wait is 0.5 second in Debian 10 running on I7-4770 @ 3.4 GHz
  setupstart = micros();                 // time to reach the setup() routine
  startserial = millis();                // use startserial as a millis() counter
  while (!Serial && (millis() - startserial < 10000));
  serialstart = (Serial);                // true if timeout not reached
  startserial = micros() - setupstart;   // time for Serial to come up in microseconds
  Serial.println("\nsetup() - 2");       // will not be seen

  // Serial is up but the USB connection is not yet completed,
  // Waiting for a USB connection
  Serial.begin(3214);                    // baud is meaningless
  serialbegin = (Serial);                // reset to false
  Serial.println("\nsetup() - 3");

  pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, LOW);

  // Wait for USB connection
  connecttimer = millis();
  while (!connected && (millis() - connecttimer < STARTUP_DELAY*1000)) {
    connected = (Serial);
  }
  connecttimer = millis() - connecttimer;

  if (connected) {
    Serial.println("\nUSB connection made.");
  }

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  Serial.println("\nloop() started");
  for (int i=10; i>0; i--) {
    report();
    Serial.printf("\nSystem reset in %d seconds\n", i);
    delay(1000);
  }
  // reset the microcontroller
  NVIC_SystemReset();
}
