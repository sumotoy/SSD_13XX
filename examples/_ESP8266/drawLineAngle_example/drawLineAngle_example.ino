/*
Draw Line Angle Example
Library has a nice function called drawLineAngle
that let you draw easily needle gauges at any angle
*/

#include <SPI.h>
#include <SSD_13XX.h>

#define __CS_TFT        4  //GPIO4 or GPIO2
#define __DC_TFT        2   //(D1)


SSD_13XX tft = SSD_13XX(__CS_TFT, __DC_TFT);


void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.drawCircle(31, 31, 27, WHITE);//draw round gauge
  tft.drawCircle(31, 31, 26, WHITE);//draw round gauge
}

void loop() {
  for (int i = 0; i <= 360; i++) {
    if (i > 0) tft.drawLineAngle(31, 31, i - 1, 25, BLACK);//erase previous needle
    tft.drawLineAngle(31, 31, i, 25, GREEN);//draw needle
    delay(10);
  }
}

