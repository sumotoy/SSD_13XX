#include <SPI.h>
#include <SSD_13XX.h>

#include "_images/sw1.c"




#define __CS1 	10
#define __DC 	9


SSD_13XX tft1 = SSD_13XX(__CS1, __DC);


void setup()
{
	tft1.begin();
	tft1.setRotation(0);
	tft1.drawImage(0, 0, &sw1);
}

void loop()
{

}
