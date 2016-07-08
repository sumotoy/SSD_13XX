/*
	A simple example, loads 2 icons and colorize
*/

#include <SPI.h>
#include <SSD_13XX.h>

#include "_icons/spkOn.c"
#include "_icons/spkOff.c"


#if defined(ESP8266)
#define __CS1   16  //(D0)
#define __CS2   4   //(D2)
#define __DC    5   //(D1)
#else
#define __CS1 	10//10
#define __DC 	9
#endif


SSD_13XX tft1 = SSD_13XX(__CS1, __DC);



void setup()
{

	tft1.begin();
	tft1.setRotation(0);

}

void loop()
{
	tft1.drawIcon(0, 0, &spkOn,  1, GREEN, BLACK, true);
	delay(250);
	tft1.drawIcon(0, 0, &spkOff, 1, RED, BLACK, true);
	delay(250);
}
