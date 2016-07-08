/*
Images are flexible! Here's an example that cut the Image background color and set as transparent!
Caution: This will not run on Arduino UNO or similar, image is too big!
*/

#include <SPI.h>
#include <SSD_13XX.h>

#include "_images/machine.c"


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
	//
	/*
	drawImages parameters:
	-------------------------------------------
	x: origin on x
	y: origin on Y
	&image: the image
	---------- following are optional ------
	imageMode:NONE,TRANSPARENT,REPLACE,BOTH
	background color threshold:0x000 to 0xFFFF (not used if imageMode set to NONE)
	--------------------------------------------
	drawImages has a couple of optional parameters
	NONE,TRANSPARENT,REPLACE,BOTH
	background threshold.
	When set to NONE, the image will be processed as is.
	When set to TRANSPARENT, the image will ignore all pixels equal or less than choosed background threshold
	When set to REPLACE, the choosed background threshold will be replaced by current background (by using setBackground(color))
	When set to BOTH, the choosed background threshold will be replaced by current background (by using setBackground), all other pixels
	will be replaced by Foregound color (set by setForeground(color))!
	>>> --------- NOTE: If your image is too wide or tall (included positions x,y) you will see nothing! ------------- <<<
	*/
}

void loop()
{
	tft1.fillScreen(RED, BLUE);
	tft1.drawImage(0, 0, &machine);
	delay(3000);
	tft1.fillScreen(RED, BLUE);
	tft1.drawImage(0, 0, &machine, TRANSPARENT, 0x19FF);
	delay(3000);
	tft1.fillScreen(RED);
	tft1.setBackground(RED);
	tft1.drawImage(0, 0, &machine, REPLACE, 0x0011);//colors under 0x00FF will be replaced by RED
	delay(3000);
	tft1.fillScreen(RED);
	tft1.setBackground(RED);
	tft1.setForeground(CYAN);
	tft1.drawImage(0, 0, &machine, BOTH, 0x0001);
	delay(3000);
}