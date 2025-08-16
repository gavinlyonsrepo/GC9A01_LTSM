# GC9A01 Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

* [Overview](#overview)
* [Installation](#installation)
* [Dependency](#dependency)
* [Documentation](#documentation)
* [Software](#software)
	* [Examples](#examples)
	* [SPI](#spi)
* [Hardware](#hardware)
* [Tested](#tested)
* [Output](#output)
* [Notes and Issues](#notes-and-issues)

## Overview

* Name: GC9A01_LTSM
* Author: Gavin Lyons
* Description:

C++ Arduino library for a TFT SPI LCD, GC9A01 Driver.

GC9A01 is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with resolution of
240RGBx240 dots, comprising a 360-channel source driver, a 32-channel gate driver, 129,600 bytes
GRAM for graphic display data of 240RGBx240 dots, and power supply circuit.

1. Power modes, Invert, Scroll, Rotate, Bitmaps supported.
2. Hardware & software SPI
3. Tested on 240X240 Round Display
4. Arduino eco-system library.
5. 16 ASCII fonts included, fonts can easily be removed or added.
6. Advanced graphics class included.
7. Advanced frame buffer mode included.
8. Bitmaps supported: 1, 8 and 16 bit.
9. Hardware & software SPI options
10. [Project url link](https://github.com/gavinlyonsrepo/GC9A01_LTSM)

* [DataSheet](https://buydisplay.com/download/ic/GC9A01A.pdf)

## Installation

The library is included in the official Arduino library manger and the optimum way to install it is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 

## Dependency

This library requires the Arduino library 'display16_LTSM' as a dependency. display16_LTSM library contains
the graphics, bitmaps, and font methods as well as font data and bitmap test data. Its also 
where the user sets options(debug, advanced graphics and frame buffer mode).
When you install 'GC9A01_LTSM' with Arduino IDE. It should install 'display16_LTSM' as well after 
a prompt, if it does not you have to install it same way as 'GC9A01_LTSM'.
The 'display16_LTSM' project and readme is at [URL github link.](https://github.com/gavinlyonsrepo/display16_LTSM)
'display16_LTSM' is also written by author of this library. 

## Documentation

Code is commented for the 'doxygen' API generation tool.
Documents on fonts, bitmaps and graphics can be found at 
the dependency 'display16_LTSM' repository, [URL github link](https://github.com/gavinlyonsrepo/display16_LTSM)

## Software

### Examples

There are 5 example files included.

| Filename .ino | Function  | Note |
| --- | --- | --- |
| HELLO WORLD | Hello world basic use case | --- |
| MISC| Text, Graphics & Functions | dislib16 ADVANCED GRAPHICS ENABLE must be enabled for all tests to work|
| BITMAP| 1,8 & 16 bit bitmaps tests + bitmap FPS tests| Bitmap test data is stored in arrays |
| DEMO| A demo showing a gauge | dislib16 ADVANCED GRAPHICS ENABLE must be enabled |
| FRAME BUFFER | Testing frame Buffer mode | dislib16 ADVANCED SCREEN BUFFER ENABLE must be enabled user option 2 |

### SPI

In the example ino files. There are sections in "setup()" function
where user can make adjustments.

1. USER OPTION 1 GPIO, SPI_SPEED + TYPE
2. USER OPTION 2 SCREEN SECTION


*USER OPTION 1 GPIO SPI SPEED*

Two different constructors which one is called depends on 'bhardwareSPI', 
true for hardware spi, false for software SPI.

Hardware SPI:

Here the user can pass the SPI Bus freq in Hertz, Currently set to 8 Mhz,
and the Reset, chip select and data or command line. Any GPIO can be used for these.
The MOSI and CLk are tied to default MCU SPI interface GPIO.

Software SPI:

The optional GPIO software uS delay,which by default is zero.
Setting this higher can be used to slow down Software SPI
which may be beneficial on Fast MCU's.
The 5 GPIO pins used. Any GPIO can be used for these.

*USER OPTION 2 Screen size*

User can adjust screen pixel height, screen pixel width.


## Hardware

[![pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)

Connections as setup in HELLO_WORLD.ino  test file.

| TFT PinNum | Pindesc | Hardware SPI | Software SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC 3.3 | VCC 3.3|
| 2 | SCLK | MCU SPI CLK | GPIO12 |
| 3 | SDA | MCU MOSI | GPIO13 |
| 4 | A0/DC | GPIO5 | GPIO5 |
| 5 | RESET | GPIO4 | GPIO4 |
| 6 | SS/CS | GPIO15 | GPIO15 |
| 7 | VCC | VCC 3.3 | VCC 3.3|
| 8 | GND | GND | GND |


1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to MCU SPI interface.
3. Backlight on/off control is left to user.
4. NOTE: Connect LED backlight pin 1 thru a resistor to VCC.

## Tested

Tested with both software and hardware SPI on:

- **ESP32**
- **Arduino UNO R4 Minima**  
 
Compiled only (not fully hardware-tested) on:

- **Arduino UNO**
- **ESP8266**
- **STM32 “Blue Pill”**

> Some examples on low-RAM MCUs will fail( insufficient memory ), numerous fonts and bitmap data are included.  
> Frame buffer mode example requires sufficient dynamic memory for the buffer — see the README in display16_LTSM for details.


## Output

[![ Demo pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)
