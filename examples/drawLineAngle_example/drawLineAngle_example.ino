/*
Draw Line Angle Example
Library has a nice function called drawLineAngle
that let you draw easily needle gauges at any angle
*/

#include <SPI.h>
#include <SSD_13XX.h>

#define __CS1 	10
#define __DC 	9


SSD_13XX tft = SSD_13XX(__CS1, __DC);



void setup() {

  tft.begin();
  tft.drawCircle(tft.height()/2, tft.height() / 2, tft.height() / 2-4, 0xFFFF);//draw round gauge
  tft.drawCircle(tft.height() / 2, tft.height() / 2, tft.height() / 2-5, 0xFFFF);//draw round gauge
}

void loop() {
  for (int i = 0; i <= 360; i++) {
    if (i > 0) tft.drawLineAngle(tft.height() / 2, tft.height() / 2, i - 1, tft.height() / 2-6, 0x0000);//erase previous needle
    tft.drawLineAngle(tft.height() / 2, tft.height() / 2, i, tft.height() / 2-6, GREEN);//draw needle
    delay(10);
  }
}

