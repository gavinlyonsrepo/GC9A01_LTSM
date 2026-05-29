/*!
	@file   DEMO_TWO.ino
	@author Gavin Lyons
	@brief  Example file for GC9A01_LTSM arduino library. demo.
	        Displays a circular volume-knob style arc on a 240x240 round display.
	        Demonstrates three phases:
	          1. Ramp up   – value increases 0 -> 99
	          2. Ramp down – value decreases 99 -> 0
	          3. Jitter    – value wanders randomly within +/- JITTER_RANGE
	@note   See USER OPTIONS 1-2 in SETUP function
			    If dislib16_ADVANCED_GRAPHICS_ENABLE is not enabled it will not compile. 
          This setting is in display_LTSM  , display16_common_LTSM.hpp ,  User Options section.
	@test
		-# Test 410 Volumne Knob demo with color gradient
*/

// libraries
#include "GC9A01_LTSM.hpp"
// Fonts needed
#include <fonts_LTSM/FontSevenSeg_LTSM.hpp>

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif



GC9A01_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software
#define TEST_DELAY1 1000   // mS

// Volume knob geometry
#define TFT_WIDTH 240
#define TFT_HEIGHT 240
#define KNOB_ARC_THICKNESS 10
#define KNOB_START_ANGLE 135.0f  // arc begins bottom-left
#define KNOB_SWEEP_ANGLE 270.0f  // total sweep in degrees
#define KNOB_BG_COLOR 0x4208     // dark gray
// Ramp timing
#define RAMP_STEP_DELAY_MS 20  // delay between each value step during ramp
// Jitter phase
#define JITTER_RANGE 8    // adjust as desired
#define JITTER_STEPS 50  // number of jitter movements
#define JITTER_STEP_DELAY_MS 400

void setup(void) {
  Serial.begin(38400);
  delay(1000);
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myTFT.TFTsetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 13;
    int8_t SCLK_TFT = 12;
    myTFT.TFTsetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT);
  // ===
  myTFT.TFTGC9A01Initialize();
  Serial.println("Start");
}

// MAIN loop
void loop(void) {
  volumeKnobDemo();
  delay(TEST_DELAY1);
  EndTests();
}
// End OF MAIN

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void) {
  myTFT.TFTPowerDown();
  Serial.println("End");
  while (1) {};
}

/*!
	@brief Map a value 0-99 to a colour cycling green -> yellow -> red
*/
static uint16_t knobColor(uint8_t value) {
  // reuse the library helper if available, otherwise simple two-stop blend
  uint8_t colorIndex = static_cast<uint8_t>(value * 127 / 99);
  return generateColor(colorIndex);
}

/*!
	@brief Draw value text centred in the knob
*/
static void drawKnobValue(uint8_t value) {
  const uint16_t centerX = TFT_WIDTH / 2;
  const uint16_t centerY = TFT_HEIGHT / 2;
  char buf[4];
  snprintf(buf, sizeof(buf), "%02d", value);
  myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
  myTFT.setFont(FontSevenSeg);
  myTFT.setCursor(centerX - 24, centerY - 16);
  myTFT.print(buf);
}

/*!
	@brief Update the knob arc 
*/
static void updateKnobArc(uint8_t oldValue, uint8_t newValue) {
  const uint16_t centerX = TFT_WIDTH / 2;
  const uint16_t centerY = TFT_HEIGHT / 2;
  const uint16_t radius = (TFT_HEIGHT - 30) / 2;

  // Use rounding for more consistent pixel coverage
  float oldEndAngle = KNOB_START_ANGLE + (KNOB_SWEEP_ANGLE * oldValue / 99.0f);
  float newEndAngle = KNOB_START_ANGLE + (KNOB_SWEEP_ANGLE * newValue / 99.0f);

  if (newValue > oldValue) {
    // Increasing: draw with small overlap to cover any gaps
    uint16_t arcColor = knobColor(newValue);
    myTFT.drawArc(centerX, centerY, radius, KNOB_ARC_THICKNESS,
                  oldEndAngle - 3.0f, newEndAngle + 2.0f, arcColor);  // overlap
  } else if (newValue < oldValue) {
    // Decreasing: erase with extra thickness + overlap
    myTFT.drawArc(centerX, centerY, radius, KNOB_ARC_THICKNESS + 3,
                  newEndAngle - 3.0f, oldEndAngle + 3.0f, myTFT.C_BLACK);
  }
}

/*!
	@brief Demo
*/
void volumeKnobDemo(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  Serial.println("Volume Knob Demo Start");
  uint8_t value = 0;
  uint8_t lastValue = 0;

  // --- Phase 1: ramp up 0 -> 99 (gradient) ---
  Serial.println("Phase 1: ramp up");
  for (value = 0; value <= 99; value++) {
    updateKnobArc(lastValue, value);
    drawKnobValue(value);
    lastValue = value;
    delay(RAMP_STEP_DELAY_MS);
  }
  delay(TEST_DELAY1);

  // --- Phase 2: ramp down 99 -> 0 ---
  Serial.println("Phase 2: ramp down");
  for (value = 99; value > 0; value--) {
    updateKnobArc(lastValue, value);
    drawKnobValue(value);
    lastValue = value;
    delay(RAMP_STEP_DELAY_MS);
  }
  updateKnobArc(lastValue, 0);  // final step
  drawKnobValue(0);
  delay(TEST_DELAY1);

  // --- Phase 3: jitter ---
  Serial.println("Phase 3: jitter");
  value = 0;
  lastValue = 0;
  for (uint8_t v = 0; v <= 50; v++) {
    updateKnobArc(lastValue, v);
    lastValue = v;
  }
  value = 50;
  drawKnobValue(value);

  for (uint16_t i = 0; i < JITTER_STEPS; i++) {
    int8_t delta = (int8_t)(random(0, JITTER_RANGE * 2 + 1) - JITTER_RANGE);
    int16_t target = (int16_t)value + delta;
    if (target < 0) target = 0;
    if (target > 99) target = 99;

    while (value != (uint8_t)target) {
      uint8_t prev = value;
      if (value < (uint8_t)target) value++;
      else value--;

      updateKnobArc(prev, value);
      drawKnobValue(value);
      delay(JITTER_STEP_DELAY_MS);
    }
  }

  Serial.println("Volume Knob Demo End");
  myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief This function is designed to return a color in the 16-bit 
		RGB format based on the input value, creating a smooth transition 
		between colors in different stages.
	@param value input value 1-127
	@return 16 bit color
	@details The gradient is calculated by 
		cycling through different color transitions as val increases, 
		allowing for smooth visual effects such as changing hues 
		or creating rainbow-like effects on an LCD. 1-127 will
		This will create a gradient color between blue and red based on the value.
*/
uint16_t generateColor(uint8_t value) {
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
  uint8_t segment = value / 32;
  switch (segment) {
    case 0:
      red = 0;
      green = 2 * (value % 32);
      blue = 31;
      break;
    case 1:
      red = 0;
      green = 63;
      blue = 31 - (value % 32);
      break;
    case 2:
      red = value % 32;
      green = 63;
      blue = 0;
      break;
    case 3:
      red = 31;
      green = 63 - 2 * (value % 32);
      blue = 0;
      break;
  }
  return (red << 11) + (green << 5) + blue;
}
/// @endcond
