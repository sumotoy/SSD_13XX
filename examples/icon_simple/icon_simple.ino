#include <SPI.h>
#include <SSD_13XX.h>

#include "_icons/wifi.c"


#define __CS1 	10
#define __DC 	9


SSD_13XX tft1 = SSD_13XX(__CS1, __DC);


void setup()
{
	tft1.begin();
	tft1.setRotation(0);
	tft1.drawIcon(0, 0, &wifi);
}

void loop()
{

}
