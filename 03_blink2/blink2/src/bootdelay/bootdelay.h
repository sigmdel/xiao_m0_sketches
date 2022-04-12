#pragma once

/*
 * Delay function meant to be placed at the beginning of the startup() function.
 *
 *   The function will return when a USB connection is made or a timeout has expired.
 *
 *   flashperiod (milliseconds)  on/off time of the yellow LED while running, 0 (default) means no flashing.
 *   timeout     (seconds)       total time until the routine exits (default 8).
 *   mindelay    (seconds)       minimum delay before the function exits (default 5).
 *
 *   The function returns True if a USB connection was established.
 *
 * The default 5 second minimum delay should enough time to start uploading new firmware and and the
 * 8 seconds timeout period should not be too long a wait until.
 *
 *  startupDelay(100, 0, 8);  // useful when debugging new firmware using USBSerial.print()
 *  startupDelay();           // should be ok in normal operation
 *
 * If flashperiod > 0 but less than 100, it will be changed to 100 ms.
 *
 * Changes the mode of the LED_BUILTIN pin (yellow LED) to INPUT if flashperiod > 0.
 */
bool startupDelay(uint16_t flashperiod = 0, uint16_t timeout = 8, uint16_t mindelay = 5);
