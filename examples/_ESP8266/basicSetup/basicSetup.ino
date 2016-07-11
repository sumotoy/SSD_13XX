#include <SPI.h>
#include <SSD_13XX.h>


#define __CS  4  //GPIO4 or GPIO2
#define __DC  5   //(D1)

/*
 SCLK:D5
 MOSI:D7
*/


SSD_13XX tft = SSD_13XX(__CS, __DC);

void setup() {
  tft.begin();
  tft.print("hello World");
}

void loop(void) {
}