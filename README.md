# xiao_m0_sketches

**Source code that accompanies the april 2022 version of [Overview of the SAMD21 Arm Cortex-M0+ Based Seeeduino XIAO](https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_01_en.html)**.

![XIAO M0 Pinout](images/xiao.jpg)

## Introduction

It did not make sense to create a repository for each of these twenty projects so it will be necessary to download the `ZIP` archive or clone the full set.

It should be possible to compile each project in the Arduino IDE or in PlatformIO. Each 
project should have no unresolved dependencies, meaning that all needed libraries are included. In other words, there should not be any need to install anything additional. Where a private library is included, only the necessary files were added. Please obtain the full library from the original source if it is to be used in another project.

<!--
Supporting both environments as seamlessly as possible means taking care of some constraints.

1. Sketch Extension
  - The Arduino environment requires that the filename of a sketch ends with the `.ino` extension. 
  - C/C++ IntelliSense, which handles code completion in PlatformIO, does not support `.INO` files.

2. File and Directory Names
  -  The Arduino environment requires that a sketch (with the `.ino` extension) be contained in a directory with the same name less the extension.
  -  PlatformIO expects that the source code will be in a subdirectory named `src`.

3. Library location
  -  The Arduino environment can find private libraries but only if they are in a subdirectory named `src` within the directory containing the `.ino` file.
  -  PlatformIO expects that the source code will be in a subdirectory named `lib`.
  
Solution 1: Move the content of the `project_name.ino` file to `main.cpp` and let `project_name.ino` contain only comments.

Solution 2: Respect the Arduino requirement and add a `src_dir` option in the `platformio.ini` configuration file.
-->

## List of Projects      

| Project | Section in *Overview... XIAO* |
| ---     | ---                           |
| **01_blink** | [7. Only a Blink Sketch, But Do Read Me](https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_01_en.html#blink) |
| **02_startup** | [8. A Proper `setup()` Function](https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_01_en.html#setup ) |
| **03_blink2** | [8. A Proper `setup()` Function](https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_01_en.html#setup ) |
| **04_analog_In** | [10. Analogue Input and Output](https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_01_en.html#analog_io ) |
| **05_AnalogCorrection** | [10.1 Analogue Input Calibration](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#analog_calibration ) |
| **06_ldr_in** | [10.2 Analogue Measure of Light with an LDR](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#analog_in ) |
| **07_freetouch** | [10.3 Capacitance Touch Sensor](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#touch ) |
| **08_testtouch** | [10.3 Capacitance Touch Sensor](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#touch ) |
| **09_dac2adc** | [10.4 Analogue Output](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#analog_out ) |
| **10_digitalOut** | [11.1 Digital Output](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#digital_output ) |
| **11_pwm** | [11.2 Pulse Width Modulation](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#xiao_pwm ) |
| **12_digitalIn** | [11.3 Digital Input](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#digital_input ) |
| **13_dac2digIn** | [11.3 Digital Input](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#digital_input ) |
| **14_interrupts** | [12. Interrupts](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#interrupts ) |
| **15_timers** | [13. Timers](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#timers ) |
| **16_basicwdt** | [14. Watchdog](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#wdt ) |
| **17_uart** | [15.1 UART](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#uart ) |
| **18_spi_test** | [15.2 SPI](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#spi ) |
| **19_i2c_scan** | [15.3 I²C](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#i2c ) |
| **20_i2c_clock** | [15.3 I²C](http://localhost/michel/ha/xiao/seeeduino_xiao_01_en.html#i2c ) |

## License

Copyright 2022, Michel Deslierres. No rights reserved. 

While the copyright pertaining to included libraries must be respected, all the code by Michel Deslierres in this repository is in the public domain. In those jurisdictions where this may be a problem, the [BSD Zero Clause License](https://spdx.org/licenses/0BSD.html) applies.
