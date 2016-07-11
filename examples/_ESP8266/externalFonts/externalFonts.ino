/*
This sketch shows how to use other fonts
and some new text commands

 */
#include <SPI.h>
#include <SSD_13XX.h>
#include "_fonts/unborn_small.c"


#define __CS  4  //GPIO4 or GPIO2
#define __DC  2   //(D1)

SSD_13XX tft = SSD_13XX(__CS, __DC);//we dont use rst, tie reset pin to 3v3

void setup() {
  Serial.begin(115200);
  Serial.print("\nstarted\n");
  tft.begin();
  /*
   after this command the library has already set several
   things as default:
   tft.clearScreen();//screen cleared with black
   tft.setRotation(0);//no rotation
   tft.setTextScale(1);//no scale
   tft.setCursor(0,0);
   tft.setTextWrap(true);//text will wrap to a new line automatically
   tft.setTextColor(WHITE,WHITE);//background transparent
   //default internal font loaded arial_x2
   This allow you to start writing immediately without
   need to set anything.
   */
  tft.setFont(&unborn_small);//this will load the font
  tft.println("Hello world!");//this will add a new line as well
  tft.setTextScale(3);//now multiply x 3 the scale
  tft.println("Hello world!");//the wrap on will split words in 2 lines
  tft.setInternalFont();//now switch to internal font
  tft.setTextScale(2);//smaller
  tft.setFontInterline(2);//this will be active after the println!
  tft.println("Hello world!");
  tft.setCharSpacing(1);//add an extra pixel between chars
  tft.setTextColor(WHITE, BLUE); //set a background inside font
  tft.println("Hello world!");
}

void loop() {

}