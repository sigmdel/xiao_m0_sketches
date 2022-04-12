/* i2c_clock

  Digital clock using
    1. Seeeduino XIAO
    2. 0.96" OLED display
    3. DS3231 real time clock (RTC)

  Communication between the microcontroller and the other two devices
  is via the I²C bus.

  Two extra libraries are used for each device.
    1. SSD1306Ascii library by Bill Greiman (greiman) @https://github.com/greiman/SSD1306Ascii
    2. RTC by Michael C. Miller (Makuna) @https://github.com/Makuna/Rtc

  The directory ./src contains cut-down versions of these libraries which will make it
  possible to compile and run the program without installing the libraries. Please use
  the full libraries in other projects.
*/

#include <Arduino.h>
#include <Wire.h>

#include "src/RTC/src/RtcDS3231.h"
#include "src/SSD1306Ascii/src/SSD1306Ascii.h"
#include "src/SSD1306Ascii/src/SSD1306AsciiWire.h"

// -- OLED params --

// set to true if the I2C pins are above the display area
// set to false if the I2C pins are below the display area
#define PINS_AT_TOP false

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1


// -- RTC params --

const char* dow[] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char* month[] = {
  "", "January", "February", "March", "April", "May", "June", "July",
  "August", "September", "October", "November", "December"
};

#define TIME_STR "%02d:%02d", now.Hour(), now.Minute()
#define DATE_STR "%s %d, %d", month[now.Month()], now.Day(), now.Year()
#define ERROR_STR  "** error **"


RtcDS3231<TwoWire> rtc(Wire);

SSD1306AsciiWire oled;

void setup() {
  delay(4000);
  // nothing written to serial monitor, start right away!

  Wire.begin();
  Wire.setClock(400000L);

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!rtc.GetIsRunning()) rtc.SetIsRunning(true);

  if (rtc.GetDateTime() < compiled) rtc.SetDateTime(compiled);

  // never assume the rtc was last configured by you, so
  // just clear them to your needed state
  rtc.Enable32kHzPin(false);
  rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);


#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif

 #if (PINS_AT_TOP != true)
  oled.displayRemap(true);
#endif
}

// center the given string in the display
void center(char* str) {
  oled.setCol( (oled.displayWidth() - oled.strWidth(str))/2 );
  oled.print(str);
}

int prevminute = -1;

void loop () {
  // communicate with RTC over I²C
  RtcDateTime now = rtc.GetDateTime();

  // wait for a change in the minute
  while (now.Minute() == prevminute)  {
    delay(5);
    now = rtc.GetDateTime();
  }
  prevminute = now.Minute();

  // communicate with display over I²C
  oled.clear();
  oled.setFont(Verdana_digits_24);

  char strBuffer[28];

  // display time
  sprintf(strBuffer, TIME_STR);
  oled.setRow(0);
  center(strBuffer);

  oled.setFont(Verdana12);

  // display day of week
  strcpy(strBuffer, dow[now.DayOfWeek()]);
  oled.setRow(4);
  center(strBuffer);
  oled.println();

  // display date
  sprintf(strBuffer, DATE_STR);
  center(strBuffer);

  delay(59000); // wait almost a minute before repeating
}
