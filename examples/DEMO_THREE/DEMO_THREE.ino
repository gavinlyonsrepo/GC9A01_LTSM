/*!
	@file   DEMO_THREE.ino
	@author Gavin Lyons
	@brief  Example file for GC9A01_LTSM arduino library.
	        Displays an analog clock on a 240x240 round display.
	        Time is derived from millis() with a user-settable start time —
	        no RTC module required.
	@note   See USER OPTIONS 1-3 in SETUP function.
	        If dislib16_ADVANCED_GRAPHICS_ENABLE is not enabled it will not compile.
	        This setting is in display_LTSM , display16_common_LTSM.hpp , User Options section.
	@test
	        -# Test 420 Clock Demo
*/

// libraries
#include "GC9A01_LTSM.hpp"
// Fonts needed
// n/a analog clock

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif

#define TFT_WIDTH 240
#define TFT_HEIGHT 240
#define TEST_DELAY1 1000  // mS
GC9A01_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software
// Millisecond timestamp captured at setup(), used to derive elapsed seconds
static unsigned long bootMillis = 0;


void setup(void) {
	Serial.begin(38400);
	delay(1000);
	// === USER OPTION 1 SPI SPEED + TYPE ===
	int8_t DC_TFT = 5;
	int8_t RST_TFT = 4;
	int8_t CS_TFT = 15;
	if (bhardwareSPI == true) {
		uint32_t TFT_SCLK_FREQ = 8000000;
		myTFT.TFTsetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
	} else {
		uint16_t SWSPICommDelay = 0;
		int8_t SDIN_TFT = 13;
		int8_t SCLK_TFT = 12;
		myTFT.TFTsetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	}
	// ===
	// === USER OPTION 2 Screen Setup ===
	myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT);
	// ===
	// === USER OPTION 3 Clock Start Time (12-hour, 0-based hour) ===
	const int START_HOUR = 10;  // 0–11
	const int START_MINUTE = 30;
	const int START_SECOND = 0;
	const int RUNTIME = 70; // Seconds
	// ===
	myTFT.TFTGC9A01Initialize();
	Serial.println("Start");
	bootMillis = millis();
	analogClockDemo(RUNTIME, START_HOUR, START_MINUTE, START_SECOND);
	EndTests();
}

void loop(void) {

}

/*!
	@brief Stop testing and power down the TFT
*/
void EndTests(void) {
	myTFT.TFTPowerDown();
	Serial.println("End");
	while (1) {}
}

/*!
	@brief Derive hour (0-11), minute, second from millis() + user start time
	@param hour   [out] current hour   0-11
	@param minute [out] current minute 0-59
	@param second [out] current second 0-59
	@param startH  User-set start hour   (USER OPTION 3)
	@param startM  User-set start minute (USER OPTION 3)
	@param startS  User-set start second (USER OPTION 3)
*/
static void getCurrentTime(int &hour, int &minute, int &second,
                           int startH, int startM, int startS) {
	unsigned long startSeconds = (unsigned long)startH * 3600UL
	                             + (unsigned long)startM * 60UL
	                             + (unsigned long)startS;
	unsigned long elapsed = (millis() - bootMillis) / 1000UL;
	unsigned long totalSeconds = (startSeconds + elapsed) % 43200UL;  // 12-hour wrap
	second = (int)(totalSeconds % 60UL);
	minute = (int)((totalSeconds / 60UL) % 60UL);
	hour = (int)((totalSeconds / 3600UL) % 12UL);
}


/*!
	@brief Draw gauge tick markers around the clock face
	@param centerX     Centre X of clock
	@param centerY     Centre Y of clock
	@param radius      Clock radius in pixels
	@param startAngle  First tick angle in degrees
	@param endAngle    Last tick angle in degrees
	@param scaleFactor Inner radius = radius / scaleFactor
	@param color       Tick colour
*/
void drawGaugeMarkers(uint16_t centerX, uint16_t centerY, uint16_t radius,
                      int startAngle, int endAngle,
                      float scaleFactor, uint16_t color) {
	float angleRad, innerX, innerY, outerX, outerY;
	for (int angle = startAngle; angle <= endAngle; angle += 30) {
		angleRad = angle * (PI / 180.0f);
		innerX = cosf(angleRad) * (radius / scaleFactor);
		innerY = sinf(angleRad) * (radius / scaleFactor);
		outerX = cosf(angleRad) * radius;
		outerY = sinf(angleRad) * radius;
		myTFT.drawLine(1 + centerX + (int16_t)innerX, 1 + centerY + (int16_t)innerY,
		               1 + centerX + (int16_t)outerX, 1 + centerY + (int16_t)outerY,
		               color);
	}
}


/*!
	@brief Analog clock demo
	@param countLimit Number of seconds to run before returning
	@param startH     Clock start hour   (USER OPTION 3)
	@param startM     Clock start minute (USER OPTION 3)
	@param startS     Clock start second (USER OPTION 3)
*/
void analogClockDemo(uint16_t countLimit, int startH, int startM, int startS) {
	myTFT.fillScreen(myTFT.C_BLACK);
	Serial.print("Analog Clock Demo Start, runs for seconds: ");
	Serial.println(countLimit);

	const int radius = (TFT_HEIGHT - 10) / 2;
	const int centerX = TFT_WIDTH / 2;
	const int centerY = TFT_HEIGHT / 2;

	// Draw static clock face
	drawGaugeMarkers(centerX, centerY, radius, 0, 360, 1.3f, myTFT.C_WHITE);
	drawGaugeMarkers(centerX, centerY, radius, 15, 360, 1.1f, myTFT.C_TAN);
	myTFT.drawArc(centerX, centerY, radius, 5, 0.0f, 360.0f, myTFT.C_BROWN);
	myTFT.fillCircle(centerX, centerY, 5, myTFT.C_CYAN);

	// Hand tip positions — track these so we can erase by redrawing in black
	uint16_t xSecond = centerX, ySecond = centerY;
	uint16_t xMinute = centerX, yMinute = centerY;
	uint16_t xHour = centerX, yHour = centerY;

	int lastSecond = -1;

	while (countLimit > 1) {
		int hour, minute, second;
		getCurrentTime(hour, minute, second, startH, startM, startS);

		if (second != lastSecond) {
			float angle;
			// Hour hand
			myTFT.drawLine(centerX, centerY, xHour, yHour, myTFT.C_BLACK);
			angle = (2.0f * PI / 12.0f) * hour;
			xHour = (uint16_t)(centerX + (radius / 2) * sinf(angle));
			yHour = (uint16_t)(centerY - (radius / 2) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xHour, yHour, myTFT.C_RED);
			// Minute hand
			myTFT.drawLine(centerX, centerY, xMinute, yMinute, myTFT.C_BLACK);
			angle = (2.0f * PI / 60.0f) * minute;
			xMinute = (uint16_t)(centerX + (2 * radius / 3) * sinf(angle));
			yMinute = (uint16_t)(centerY - (2 * radius / 3) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xMinute, yMinute, myTFT.C_GREEN);
			// Second hand
			myTFT.drawLine(centerX, centerY, xSecond, ySecond, myTFT.C_BLACK);
			angle = (2.0f * PI / 60.0f) * second;
			xSecond = (uint16_t)(centerX + (radius - 30) * sinf(angle));
			ySecond = (uint16_t)(centerY - (radius - 30) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xSecond, ySecond, myTFT.C_YELLOW);
			// Redraw centre pip so hands don't erase it
			myTFT.fillCircle(centerX, centerY, 5, myTFT.C_CYAN);
			Serial.printf("%02d:%02d:%02d\n", hour, minute, second);
			lastSecond = second;
			countLimit--;
		}
		delay(10);
	}

	myTFT.fillScreen(myTFT.C_BLACK);
	Serial.println("Clock Demo End");
}

/// @endcond
