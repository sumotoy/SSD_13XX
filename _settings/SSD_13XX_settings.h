/*=========================================================================================
	Part of SSD_13XX library
    Copyright (c) 2014/2015/2016, .S.U.M.O.T.O.Y., coded by Max MC Costa.

    SSD_13XX Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSD_13XX Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
	---------------------------------------------------------------------------------------
	User settings
===========================================================================================*/
#ifndef _SSD_13XX_USETT_H_
#define _SSD_13XX_USETT_H_
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
									USER SETTINGS
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*--------------------------------------------------------------------------------
Here you choose the display file for your display.
You must select just ONE and comment the others.
Default: #include "../_display/SSD_1331_96x64.h"
(uncomment just one below...)
----------------------------------------------------------------------------------*/
	#include "../_display/SSD_1331_96x64.h"
	//#include "../_display/SSD_1331_REDPCB.h"
	//#include "../_display/SSD_1332_96x64.h"
	//#include "../_display/SSD_1351_128x128.h"
/*--------------------------------------------------------------------------------
- Size Reducing (decrease slight performances) -
Ignored for Teensy 3.x, DUE
Small CPU like UNO have very small resources and code optimizations uses lot of.
Uncomment _SSD_SIZEOPTIMIZER will decrease space needed by code but some performance
will suffer a bit, however it can be usefult in many cases!
Default:uncommented (automatically enabled for some CPU)
----------------------------------------------------------------------------------*/
#if !defined(__MK20DX128__) && !defined(__MK20DX256__) && !defined(__SAM3X8E__) && !defined(__MK64FX512__) && !defined(__MK66FX1M0__)// && !defined(ESP8266)
	#define _SSD_SIZEOPTIMIZER
#endif
/*--------------------------------------------------------------------------------
- Default Font -
To change default font, you must:
	_SSD_DEF_FONT_PATH "the complete path of the font" (default:"_fonts/defaultFont.c")
	_SSD_DEF_FONT_NAME	the_unique_name_of_the_font (default:defaultFont)
To avoid any font, just uncomment the 2 defines and a blank font will be loaded (almost 0 resources)
Default: (defaultFont = arial_2x)
#define _SSD_DEF_FONT_PATH			"_fonts/defaultFont.c"
#define _SSD_DEF_FONT_NAME			defaultFont
----------------------------------------------------------------------------------*/
	#define _SSD_DEF_FONT_PATH			"_fonts/defaultFont.c"
	#define _SSD_DEF_FONT_NAME			defaultFont
/*--------------------------------------------------------------------------------
- Default Display Rotation -
This parameter can be changed in your code but here you can force orientation
Default:0
----------------------------------------------------------------------------------*/
#define _SSD_DEF_ROTATION			0
/*--------------------------------------------------------------------------------
- Default Background Color & Default Foreground Color -
When display turns on, it will set the defaul background and foreground.
the default background and foreground affects several other function
when you forget to assign color, for example clearScreen() will clear screen to black
Assigning setBackground(0xF800), red, will result in a red screen when cleared.
Default:BLACK & WHITE
----------------------------------------------------------------------------------*/
#define _SSD_DEF_BACKGROUND		BLACK
#define _SSD_DEF_FOREGROUND		WHITE
/*--------------------------------------------------------------------------------
- Teensy LC Fast CS Port option -
Teensy LC optional Direct Port vs digitalWriteFast methods
If you have any issues ONLY with Teensy LC and other SPI devices that share
the same SPI lines try to comment the line nelow...
The library default uses Direct Port Manipulation (that it's slight faster)
Default:uncommented
----------------------------------------------------------------------------------*/
#if defined(__MKL26Z64__)
	#define _TEENSYLC_FASTPORT
#endif
/*--------------------------------------------------------------------------------
- ESP8266 Faster SPI -
This force library to use the SPI.write method instead the legacy SPI.transfer.
As result is much faster. (Thanks Reaper7)
Default:uncommented
----------------------------------------------------------------------------------*/
#if defined(ESP8266)
	#define _ESP8266_SPIFAST
#endif
/*--------------------------------------------------------------------------------
- ESP8266 Compatibility mode -
This force library to use an alternative way to trigger ESP8266 GPIO, if you uncomment
the line it will use the standard digitaWrite wich is slow, this help debugging.
NOTE: uncomment this, code is MUCH slower!
Default:commented
----------------------------------------------------------------------------------*/
#if defined(ESP8266)
	//#define _ESP8266_STANDARDMODE
#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*---------------------------------------------------------------------------------
 							 END OF USER SETTINGS
----------------------------------------------------------------------------------*/
static const uint32_t	SSD_CGRAM		=	SSD_WIDTH * SSD_HEIGHT;
#endif
