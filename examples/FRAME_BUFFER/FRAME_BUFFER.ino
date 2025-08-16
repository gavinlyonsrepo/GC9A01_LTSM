/*!
	@file   FRAME_BUFFER.ino
	@author Gavin Lyons
	@brief  Example file for GC9A01_LTSM arduino librarySee USER OPTIONS 0-2 in SETUP function
			    dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
			    or it will not compile , requires usable heap memory of (240X240X2) = 115,200 bytes
	@test
		-# Test 1500 Frame buffer mode.
*/

// libraries
#include "GC9A01_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontArialRound_LTSM.hpp"

/// @cond

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif

GC9A01_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

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
  uint16_t TFT_WIDTH = 240;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 240;  // Screen height in pixels
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT);
  // ===
  myTFT.TFTGC9A01Initialize();

  // set up buffer
  if (myTFT.setBuffer() != DisLib16::Success){
    Serial.println("buffer not set");
    while(1){};
  }
  // set to use pixel mode for text                 
  myTFT.setTextCharPixelOrBuffer(true); 
  Serial.println("Start");
}


// MAIN loop
void loop(void) {
  Test1500();
  EndTests();
}
// End OF MAIN 

void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.destroyBuffer();
  myTFT.TFTPowerDown();
  Serial.println("End");
  while (1) {};
}

void ScreenReset(void) {
  myTFT.writeBuffer();
  delay(2500);
  myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1500(void) {
  Serial.println("Test1");
  myTFT.clearBuffer(myTFT.C_RED);
  myTFT.writeBuffer();
  delay(1000);
  myTFT.clearBuffer(myTFT.C_GREEN);
  myTFT.writeBuffer();
  delay(1000);
  myTFT.clearBuffer(myTFT.C_BLUE);
  myTFT.writeBuffer();
  delay(1000);

  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setCursor(15, 100);
  myTFT.setFont(FontArialRound);
  myTFT.print("Buffer Mode");
  myTFT.writeBuffer();
  delay(5000);
}

/// @endcond
