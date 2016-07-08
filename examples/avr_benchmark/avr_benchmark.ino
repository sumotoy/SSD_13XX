/*
	AVR Benchmark version 0.5
*/


#include <SPI.h>
#include <SSD_13XX.h>


#define __CS1 	10
#define __DC 	9


SSD_13XX tft = SSD_13XX(__CS1, __DC);

unsigned long t_start,t_end;
uint8_t rotation;
uint8_t x,y;
uint8_t i;


void setup()
{
	Serial.begin(38400);
	tft.begin();

}

void clearS()
{
	tft.clearScreen();
}

void waitAlittle()
{
	delay(500);
}


void testFillScreen() {
	clearS();
	t_start = micros();
	tft.fillScreen(RED);
	t_end =  micros() - t_start;
	Serial.println(t_end);
}

void testText() {
	clearS();
	t_start = micros();
	tft.setCursor(0, 0);
	tft.setTextColor(WHITE);
	tft.setTextScale(1);
	tft.println("Hello World!");
	tft.setTextColor(YELLOW);
	tft.setTextScale(2);
	tft.println(1234.56);
	tft.setTextColor(RED);
	tft.setTextScale(3);
	tft.println(0xDEAD, HEX);
	tft.println();
	tft.setTextColor(GREEN);
	tft.setTextScale(4);
	tft.println("Hello");
	t_end = micros() - t_start;
	Serial.println(t_end);
}



void testLines(void) {
	int           x2, y2;

	x = y = 0;
	y2 = tft.height()-1;
	t_start = micros();
	for (x2 = 0; x2 < tft.width(); x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = tft.width()-1;
	for (y2 = 0; y2 < tft.height(); y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end = micros() - t_start; // fillScreen doesn't count against timing

	clearS();

	x = tft.width()-1;
	y = 0;
	y2 = tft.height()-1;
	t_start = micros();
	for (x2 = 0; x2 < tft.width(); x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = 0;
	for (y2 = 0; y2 < tft.height(); y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end += (micros() - t_start);

	clearS();

	x = 0;
	y = tft.height()-1;
	y2 = 0;
	t_start = micros();
	for (x2 = 0; x2 < tft.width(); x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = tft.width()-1;
	for (y2 = 0; y2 < tft.height(); y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end += (micros() - t_start);

	clearS();

	x = tft.width()-1;
	y = tft.height()-1;
	y2 = 0;
	t_start = micros();
	for (x2 = 0; x2 < tft.width(); x2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	x2 = 0;
	for (y2 = 0; y2 < tft.height(); y2 += 6) tft.drawLine(x, y, x2, y2, CYAN);
	t_end += (micros() - t_start);
	Serial.println(t_end);
}

void testFastLines(void) {
	t_start = micros();
	for (y = 0; y < tft.height(); y += 5) tft.drawFastHLine(0, y, tft.width()-1, RED);
	for (x = 0; x < tft.width(); x += 5) tft.drawFastVLine(x, 0, tft.height()-1, BLUE);
	t_end += (micros() - t_start);
	Serial.println(t_end);
}

void testRects(bool filled) {
	t_start = micros();
	for (i = tft.height(); i > 4; i -= 4) {
		if (filled) {
			t_start = micros();
			tft.fillRect((tft.width()/2) - (i / 2), (tft.height()/2) - (i / 2), i, i, YELLOW, BLACK);
			t_end += micros() - t_start;
		}
		tft.drawRect((tft.width()/2) - (i / 2), (tft.height()/2) - (i / 2), i, i, MAGENTA);
	}
	if (!filled) t_end += (micros() - t_start);
	Serial.println(t_end);
}


void testCircles(bool filled) {
	t_start = micros();
	for (x = 0; x < tft.width(); x += 10) {
		for (y = 0; y < tft.height(); y += 10) {
			if (filled) {
				tft.fillCircle(x, y, 5, BLUE);
			} else {
				tft.drawCircle(x, y, 5, YELLOW);
			}
		}
	}
	t_end += (micros() - t_start);
	Serial.println(t_end);
}

void testTriangles() {
	t_start = micros();
	uint8_t val = 32;
	for (i = 0; i < val; i += 4) {
		tft.drawTriangle(
			val-1, val-1 - i, // peak
			val-1 - i, val-1 + i, // bottom left
			val-1 + i, val-1 + i, // bottom right
			tft.Color565(0, 0, i));
	}
	t_end += (micros() - t_start);
	Serial.println(t_end);
}


void printSpaces(uint8_t spaces)
{
	for (i = 0; i < spaces; i++) {
		Serial.write(' ');
	}
}



void loop()
{

	for (rotation = 0; rotation < 4; rotation++) {
		clearS();
		tft.setRotation(rotation);
		Serial.print(F("\nBenchmark[rot="));
		Serial.print(rotation);
		Serial.println(F("]          (ms)"));

		Serial.print(F("Screen(F)"));
		printSpaces(16);
		clearS();
		testFillScreen();
		waitAlittle();

		Serial.print(F("Text"));
		printSpaces(21);
		clearS();
		testText();
		waitAlittle();

		Serial.print(F("Lines"));
		printSpaces(20);
		clearS();
		testLines();
		waitAlittle();

		Serial.print(F("Lines2"));
		printSpaces(19);
		clearS();
		testFastLines();
		waitAlittle();

		Serial.print(F("Rectangles"));
		printSpaces(15);
		clearS();
		testRects(false);
		waitAlittle();

		Serial.print(F("Rectangles(F)"));
		printSpaces(12);
		clearS();
		testRects(true);
		waitAlittle();

		Serial.print(F("Circles(F)"));
		printSpaces(15);
		testCircles(true);
		waitAlittle();

		Serial.print(F("Circles"));
		printSpaces(18);
		testCircles(false);
		waitAlittle();
	}

}


