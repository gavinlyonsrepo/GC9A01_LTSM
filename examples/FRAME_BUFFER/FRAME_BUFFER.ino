/*!
	@file   FRAME_BUFFER.ino
	@author Gavin Lyons
	@brief  Example file for GC9A01_LTSM arduino librarySee USER OPTIONS 0-2 in SETUP function
			    dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
			    or it will not compile:
        	This setting is in library display16_LTSM, display16_common_LTSM.hpp ,  
			    User Options section.
          Requires usable heap memory of (240X240X2) = 115,200 bytes
	@test
		-# Test 1500 Frame buffer mode.
    -# Test 601 FPS test Frame buffer mode.
*/

// libraries
#include "GC9A01_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontArialRound_LTSM.hpp"
// bitmap test data
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color1.hpp"

//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

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
  // Set rotation BEFORE setBuffer(): the buffer is laid out for the display's
  // current width and height. On a display where width != height, call
  // setBuffer() again after every rotation change.
  myTFT.TFTsetRotation(myTFT.Degrees_0); // Degrees_0 is the default, shown for clarity
  if (myTFT.setBuffer() != DisLib16::Success) {
    Serial.println("set up buffer error");
    while (1) {};
  }
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
  Test601();
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

/*!
	@brief  Test601 frame rate per second FPS ,results to serial port
*/
void Test601(void) {
   myTFT.clearBuffer(myTFT.C_BLACK);
   myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);
  // Values to count frame rate per second
  long previousMillis = 0;
  long lastFramerate = 0;
  long currentFramerate = 0;

  uint16_t count = 0;
  uint16_t seconds = 0;
  uint16_t fps = 0;

  char teststr1[] = "Test 601 FPS, Output Results to USB Serial port";
  myTFT.writeCharString(75, 75, teststr1);
  myTFT.writeBuffer();
  delay(TEST_DELAY5);
  myTFT.clearBuffer(myTFT.C_BLACK);

  while (1) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000)  // every second
    {
      fps = currentFramerate - lastFramerate;
      lastFramerate = currentFramerate;
      previousMillis = currentMillis;
      seconds++;
      if (seconds == 20)
        break;
    }
    currentFramerate++;
    count++;

    //  ** Code to test **
    myTFT.drawBitmap16Data(70, 70, MotorImage, 128, 128);
    myTFT.setCursor(55, 55);
    myTFT.print(fps);
    myTFT.writeBuffer();
    //   **
  }  // end of while

  // Report results to usb
  Serial.print("Seconds :: ");
  Serial.println(seconds);
  Serial.print("Count :: ");
  Serial.println(count);
  Serial.print("FPS :: ");
  Serial.println(fps);
  // Print to screen
  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.setCursor(40, 55);
  myTFT.print("Secs : ");
  myTFT.print(seconds);
  myTFT.setCursor(40, 75);
  myTFT.print("Count : ");
  myTFT.print(count);
  myTFT.setCursor(40, 95);
  myTFT.print("FPS : ");
  myTFT.print(fps);
  myTFT.writeBuffer();
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}
/// @endcond
