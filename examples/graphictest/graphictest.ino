/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
  NOTE: This version is the same of the ILI9340_t3 library
  here just for testing...
 ****************************************************/


#include <SPI.h>
#include <SSD_13XX.h>

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

uint8_t errorCode = 0;

SSD_13XX tft = SSD_13XX(TFT_CS, TFT_DC);

void setup() {
Serial.begin(38400);
long unsigned debug_start = millis();
while (!Serial && ((millis() - debug_start) <= 5000));
  tft.begin(false);
  delay(30);
  //the following it's mainly for Teensy
  //it will help you to understand if you have choosed the
  //wrong combination of pins!
  errorCode = tft.getErrorCode();
  if (errorCode != 0) {
	  Serial.print("Init error! ");
	  if (bitRead(errorCode, 0)) Serial.print("MOSI or SCLK pin mismach!\n");
	  if (bitRead(errorCode, 1)) Serial.print("CS or DC pin mismach!\n");
  }
  else {
	  Serial.println(F("Benchmark Sketch V1.1"));
	  tft.setTextColor(YELLOW);
	  tft.setTextScale(2);
  }
}


void loop(void) {
	if (errorCode == 0) {
		for (uint8_t rotation = 0; rotation < 4; rotation++) {
			tft.setRotation(rotation);
			Serial.println(F("Benchmark                Time (microseconds)"));

			Serial.print(F("Screen fill              "));
			Serial.println(testFillScreen());
			delay(200);

			Serial.print(F("Text                     "));
			Serial.println(testText());
			delay(600);

			Serial.print(F("Lines                    "));
			Serial.println(testLines(CYAN));
			delay(200);

			Serial.print(F("Horiz/Vert Lines         "));
			Serial.println(testFastLines(RED, BLUE));
			delay(200);

			Serial.print(F("Rectangles (outline)     "));
			Serial.println(testRects(GREEN));
			delay(200);

			Serial.print(F("Rectangles (filled)      "));
			Serial.println(testFilledRects(YELLOW, MAGENTA));
			delay(200);

			Serial.print(F("Circles (filled)         "));
			Serial.println(testFilledCircles(10, MAGENTA));

			Serial.print(F("Circles (outline)        "));
			Serial.println(testCircles(10, WHITE));
			delay(200);

			Serial.print(F("Triangles (outline)      "));
			Serial.println(testTriangles());
			delay(200);

			Serial.print(F("Triangles (filled)       "));
			Serial.println(testFilledTriangles());
			delay(200);

			Serial.print(F("Rounded rects (outline)  "));
			Serial.println(testRoundRects());
			delay(200);

			Serial.print(F("Rounded rects (filled)   "));
			Serial.println(testFilledRoundRects());
			delay(200);

			Serial.println(F("Done!"));
			delay(1000);
		}
	}
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.clearScreen();
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextScale(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW); tft.setTextScale(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    tft.setTextScale(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextScale(5);
  tft.println("Groop");
  tft.setTextScale(2);
  tft.println("I implore thee,");
  tft.setTextScale(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.clearScreen();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.clearScreen();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.clearScreen();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.clearScreen();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.clearScreen();
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.clearScreen();
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.clearScreen();
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.clearScreen();
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.clearScreen();
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.Color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.clearScreen();
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.Color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.Color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.clearScreen();
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.Color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.clearScreen();
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.Color565(0, i, 0));
  }

  return micros() - start;
}
