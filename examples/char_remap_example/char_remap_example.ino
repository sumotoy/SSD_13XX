/*
 An example of a font that use char remap:
 akasha_cap has only this chars +,-.0123456789:<=>?ABCDEFGHIJKLMNOPQRSTUVWXY
 but it has remap flag enabled to 1.
 Inside akasha_cap.c, at the end, this line:
 const tFont akasha_cap = { 45, akasha_cap_array,0,11, RLE_no, 1 };
 the last parameter is set to 1, this force library to remap of all minus to CAPITALs
*/

#include <SPI.h>
#include <SSD_13XX.h>

#include "_fonts/akasha_cap.c"


#if defined(ESP8266)
#define __CS1   16  //(D0)
#define __CS2   4   //(D2)
#define __DC    5   //(D1)
#else
#define __CS1 	10
#define __DC 	9
#endif


SSD_13XX tft1 = SSD_13XX(__CS1, __DC);


void setup()
{
	tft1.begin();
	tft1.setRotation(0);
	tft1.setFont(&akasha_cap);
	tft1.print("abc ABC");
}

void loop()
{

}
