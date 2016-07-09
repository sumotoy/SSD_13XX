# SSD_13XX
A very fast and full featured driver for <b>Solomon Systech SSD1331, SSD1332 and SSD1351</b> drivers designed mainly for Oled's compatible with Teensy's and many other MCU's.
I have other 2 libraries here in github but have some issues and are quite old, this will replace both.<br>

<b>Current Version:</b><br>
 - 1.0r1: SSD1331/SSD1332 tested only with various Teensy's. If not work with other CPU please report only, I NEVER tested the code with other CPU's so I expect this, but remember that is a preview and things gonna change soon.

<b>Driver description (what I should choose?):</b><br>
* <b>SSD1331</b> is a full featured driver for 96x64 oled with a lot of useful registers and an hardware accellerated standalone scroll that I never seen in other drivers. This driver is currently supported.<br>
* <b>SSD1332</b> is the 'low cost' version of SSD1331, much less features, less fast, no scrolling. This driver is currently supported.<br>
* <b>SSD1351</b> (in development)<br>
<b>SSD1331</b><br>  
![SSD1331](https://github.com/sumotoy/SSD_13XX/blob/docs/images/ssd1331.png)<br>
<b>SSD1332</b><br>  
![SSD1332](https://github.com/sumotoy/SSD_13XX/blob/docs/images/ssd1332.png)<br>
Note: for this display you need to set it for SPI and connect all pin from D2 to D7 to ground<br>
<b>How to set driver:</b><br>  
You need to open _settings/SSD_13XX_settings.h with a text editor and uncomment the driver you want to use:<br>
Es. <code>#include "../_display/SSD_1331_96x64.h"</code>

<b>Driver that will never supported:</b><br>
 - SSD_1306: This device need a completely different library since it have no internal framebuffer.<br>

<b>CPU Supported:</b><br>
 - Teensy 3.0, 3.1, 3.2, 3.4, 3.5, LC
 - Teensy 2.0 and variant, any Arduino 8 bit
 - Arduino DUE
 - ESP8266
 - SPARK (next versions)
 - Several ST (next versions)<br>

<b>Note:</b> I currently will not support Arduino Zero, it's currently poor documented and it cost a fortune, I suggest a Teensy that is much cheaper, more featured and smaller!<br>
However, this is open source an released with GNU V3 so you can fork library and add Zero support yourself as you respect the GNU licence and leave intact any comment.<br>

<b>Background:</b><br>
I got my first SSD_1331 on Ebay, find a library compatible at Adafruit but I was extremely disappointed about performances/features and contains lot of bugs, so I decided to write a new one from scratch based on my popular TFT_ILI9163C. Thanks anyway to lady Ada for supporting opensource and developers!<br>
Unfortunatly these devices are similar each others but commands and methods are really different, in particular SSD_1331/1332 uses commands to send data when most chip uses dataTransfer, so is surely not easy write an universal library even for similar chip.<br>
I have 2 old libraries here on github, but digging inside datasheet I discover that I can get much more by rewrote almost everithing. As result I got all chip features enabled and it's probably the faster driver around, much more than adafruit and works with all features at any screen rotation by using all the time his hardware accellerated primitives<br>
Commands and features are pretty similar to TFT_9163C,TFT_7735,TFT_ILI93XX last libraries here on github, of course there's differences related to hardware (OLED don't have backlight pin, etc.)

<b>Features:</b><br>
 - Blazing fast, near chip hardware limits, expecially with Teensy's.
 - Any feature enabled for any rotation, using native hardware accellerated primitives.
 - Standard SPI & SPI transaction compatible, play nice with other SPI devices.
 - Many CPU supported.
 - Ultrafast Teensy's 3.0,3.1,3.2,3.4,3.5 SPI FIFO.
 - Almost all chip features supported.
 - LGPO Font accellerated rendering.
 - Icon support.
 - Image support.
 - Gradients.
 - Icon support (as seen in my last other libraries, use same icon file format).
 - Image support (as seen in my last other libraries, use same image file format).
 - User Font support (as seen in my last other libraries, use same font file format).
 - Uses TFT_ILI9163C command format that have many features and covers commands used in many libraries so it will be easy convert code to work with this one.<br>

<b>SSD1331/32Unique commands: (differs from other libraries due different chip features)</b><br>
 - copyArea: copy a portion of screen into another location (hardware acc)
 - dimArea: dim a selected area
 - scroll: this chip has an standalone hardware scroll
 - defineScrollArea: since scroll is hardware, all is defined inside this function)
 - clearArea:
 - setColorDepth:8 or 16bit
 - setColorOrder:RGB or BGR
 - drawRect: the standard drawRect is provided, but this advanced version have borders. Hardware accellerated.
 - mode:PWRSAVE It's almost equal to sleep, display goes off
 - mode:DISP_DIM The display goes in Dim mode
 - mode:PROTECT The chip logic don't accept any commands, just mode:NORMAL to goes off of protect mode.<br>
<b>Performances (SSD1331):</b><br>
Early alpha wasnot working, only garbage on screen, it's here for reference<br>
Alpha 1..2 was using absolute screen addressing (now abandoned)<br>
Alpha 3..4 and beta's use new hardware rotation addressing. In Alpha 4 triangles rendering failed.<br>
Beta 1..2 works and are usable, all function tested at any rotation.<br>
CPU used for benchmarks: Teensy 3.2 at 96Mhz. Sketch:benchmarks_official.ino<br>
Times are in Microseconds!<br>

	|test               |Early|alpha 1|alpha 2|alpha 3|alpha 4|beta 1|beta 2|
	|-------------------|------|------|------|------|------|------|------|
	|Screen fill	|409|809|807|809|808|507|<b>406</b>|
	|Text	|3983|3907|3963|3891|2357|2378|<b>2270</b>|
	|Text2	|9564|9368|9517|9332|5047|5057|<b>4910</b>|
	|Lines	|5576|10976|10978|10974|5561|299|<b>277</b>|
	|Horiz/Vert Lines|6827|1800|1978|1964|1956|1954|<b>1956</b>|
	|Rectangles (outline)|4508|8916|8919|8915|1126|1376|<b>1123</b>|
	|Rectangles (filled)|4509|8924|8922|8920|1223|1499|<b>1214</b>|
	|Circles (filled)|3635|18787|4488|4482|4481|4479|<b>4481</b>|
	|Circles (outline)|4697|2851|2852|2849|2848|2847|<b>2847</b>|
	|Triangles (outline)|517|505|508|504|503|204|<b>183</b>|
	|Triangles (filled)|1916|11087|4284|4274|276|4274|<b>4275</b>|
	|Rounded rects (outline)|1206|2942|1619|1586|1585|1587|<b>1588</b>|
	|Rounded rects (filled)	|10271|8184|5814|5807|5036|5241|<b>5032</b>|
	|Icon Render|	| 	|   	|    	||1404|<b>1312</b>|
	
More or less, Beta 2 it's the release candidate, you can note how amazing fast in some operation, for example 406 microsec for a complete screen erase, 277 microsecs for the complete line test or the 183 microsec for the triangle test (remember that triangulation is used for 3d rendering as well, it should be fast), even text it's really fast. In comparison, for the Adafruit one I have to use milliseconds, it's amazing slow (even using the fast SPI mode). I will up a table comparison (for fun) whe I release out this one.

<b>Shit goes on:</b><br>
After SSD1331 I'm currently on SSD1332, I was suppose it's very easy but as always not! Did you think SSD1332 it's better than SSD1331? NO! The SSD1331 it's faster, better colors (the RED on SSD1332 is always orange, no matter settings!) and also cheaper. I Got the SSD1332 working but color still looks weird to me.<br>

<b>Licence:</b><br>
Licenced under GNU V3.
 
