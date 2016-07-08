
#include <SPI.h>
#include <SSD_13XX.h>

#define DELAY_BETWEEN 500


#define __CS 	10
#define __DC 	9
/*
ESP8266-----------------------------------
Use:
#define __CS  16  //(D0)
#define __DC  5   //(D1)
#define __RST 4   //(D2)

 SCLK:D5
 MOSI:D7
*/

SSD_13XX tft = SSD_13XX(__CS, __DC);

unsigned long start = 0;
unsigned long t_end = 0;

void setup() {
  Serial.begin(38400);
  start = millis ();
  while (!Serial && ((millis () - start) <= 5000)) ;
  tft.begin();
}

uint8_t rot = 0;
void loop(void) {
  test(rot);
  rot++;
  if (rot > 3) rot = 0;
}
