# SSD_13XX
A very fast and full featured driver for <b>Solomon Systech SSD1331, SSD1332, SSD1322 and SSD1351</b> drivers designed mainly for Oled's compatible with Teensy's and many other MCU's.
I have other 2 libraries here in github but have some issues and are quite old, this will replace both.<br>

<b>Current Version:</b><br>
 - 1.0r2: SSD1331/SSD1332/SSD1351 tested only with various Teensy's and ESP8266 (Have to set SPI at 79.999.999!). If not work with other CPU please report only, I NEVER tested the code with other CPU's so I expect this, but remember that is a preview and things gonna change soon.

<b>Driver description (what I should choose?):</b><br>
* <b>SSD1331</b> is a full featured driver for 96x64 oled with a lot of useful registers and an hardware accellerated standalone scroll that I never seen in other drivers. This controller is currently supported.<br>
* <b>SSD1332</b> is the 'low cost' version of SSD1331, much less features, less fast (but use some Hardware Accellerated primitives), no scrolling. This controller is currently supported.<br>
* <b>SSD1322</b> used in some wide oled's.(in development)<br>
* <b>SSD1351</b> Can drive 128x128 Oled's, doesn't have any Hardware Accellerated primitive (apart scrolling as SD1331), pretty featured. This controller is currently supported.<br>

***

<b>SSD1331</b> Currently my preferred one...<br>  
Uncomment in settings file: <code>#include "../_display/SSD_1331_96x64.h"</code>
![SSD1331](https://github.com/sumotoy/SSD_13XX/blob/docs/images/ssd1331.png)<br>
<b>NOTE:</b>works out of the box, no hardware configurations, very bright
***
<b>SSD1331 RED PCB 0.96" Color Oled</b><br>  
Uncomment in settings file: <code>#include "../_display/SSD_1331_REDPCB.h"</code>
![SSD1331b](https://github.com/sumotoy/SSD_13XX/blob/docs/images/ssd1331b.png)<br>
<b>NOTE:</b>This Oled needs to be configured for SPI (jumpers on the back) and you <b>should</b> connect D2,D3,D4,D5,D6,D7 to GND!
***
<b>SSD1332</b><br>  
Uncomment in settings file: <code>#include "../_display/SSD_1332_96x64.h"</code>
![SSD1332](https://github.com/sumotoy/SSD_13XX/blob/docs/images/ssd1332.png)<br>
<b>NOTE:</b>This Oled needs to be configured for SPI (jumpers on the back) and you <b>should</b> connect D2,D3,D4,D5,D6,D7 to GND! I will not raccomand this, red color is still orange to me.
***
<b>SSD1351 128x128</b><br>  
Uncomment in settings file: <code>#include "../_display/SSD_1351_128x128.h"</code>
![SSD1332](https://github.com/sumotoy/SSD_13XX/blob/docs/images/ssd1351.png)<br>

***

<b>How to set driver:</b><br>  
You need to open _settings/SSD_13XX_settings.h with a text editor and uncomment the driver you want to use:<br>
Es. <code>#include "../_display/SSD_1331_96x64.h"</code>

<b>Driver that will never supported:</b><br>
 - SSD_1306: This device need a completely different library since it have no internal framebuffer.<br>

***
<b>CPU Supported:</b><br>
 - Teensy 3.0, 3.1, 3.2, 3.4, 3.5, LC
 - Teensy 2.0 and variant
 - Any AVR 8 bit (UNO, etc.)
 - Arduino DUE
 - ESP8266
 - SPARK (next versions)
 - Several ST (next versions)<br>

<b>Note:</b> I currently will not support Arduino Zero, it's currently poor documented and it cost a fortune, I suggest a Teensy that is much cheaper, more featured and smaller!<br>
However, this is open source an released with GNU V3 so you can fork library and add Zero support yourself as you respect the GNU licence and leave intact any comment.<br>

***
<b>Background:</b><br>
I got my first SSD_1331 on Ebay, find a library compatible at Adafruit but I was extremely disappointed about performances/features and bugs, so I decided to write a new one from scratch based on my popular TFT_ILI9163C. Since this devices are very similar I have decided to build up an unique library.<br>
I have 2 old libraries here on github, but digging inside datasheet I discover that I can get much more by rewrote almost everithing. As result I got all chip features enabled and it's probably the faster driver around, much more than adafruit and works with all features at any screen rotation by using all the time his hardware accellerated primitives<br>
Commands and features are pretty similar to TFT_9163C,TFT_7735,TFT_ILI93XX last libraries here on github, of course there's differences related to hardware (OLED don't have backlight pin, etc.)
***
<b>Features:</b><br>
 - A display file for each oled, easy to customize.
 - Blazing fast, near chip hardware limits, expecially with Teensy's.
 - Any feature enabled for any rotation, using native hardware accellerated primitives.
 - Standard SPI & SPI transaction compatible, play nice with other SPI devices.
 - Many CPU supported.
 - Ultrafast Teensy's 3.0,3.1,3.2,3.4,3.5 SPI FIFO.
 - Fully SPI Transaction (where applicable), play nice with other SPI devices.
 - Almost all chip features supported.
 - LGPO Font accellerated rendering.
 - Icon support.
 - Image support.
 - Gradients.
 - Icon support (as seen in my last other libraries, use same icon file format).
 - Image support (as seen in my last other libraries, use same image file format).
 - User Font support (as seen in my last other libraries, use same font file format).
 - Uses TFT_ILI9163C command format that have many features and covers commands used in many libraries so it will be easy convert code to work with this one.<br>

***

<b>SSD1331/32 Unique commands: (differs from other libraries due different chip features)</b><br>
 - <b>copyArea</b>: copy a portion of screen into another location (hardware acc.)
 - <b>dimArea</b>: dim a selected area (only SSD1331)
 - <b>scroll</b>: this chip has an standalone hardware scroll (only SSD1331 and SSD1351)
 - <b>defineScrollArea</b>: since scroll is hardware, all is defined inside this function)
 - <b>moveArea</b>: It move a defined area into another place (hardware acc.)
 - <b>setColorDepth</b>:8 or 16bit
 - <b>setColorOrder</b>:RGB or BGR (only SSD1331)
 - <b>drawRect</b>: the standard drawRect is provided, but this advanced version have borders. (hardware acc.) (only SSD1331/32)
 - mode:<b>PWRSAVE</b> It's almost equal to sleep, display goes off
 - mode:<b>DISP_DIM</b> The display goes in Dim mode (only SSD1331)
 - mode:<b>PROTECT</b> The chip logic don't accept any commands, just mode:NORMAL to goes off of protect mode.(only SSD1331)<br>

***
<b>Connections:</b><br>
Of course some oled has just SPI exposed but some can be configured for parallel, I2C and so on. I'm using ONLY SPI here (so don't ask for any other protocol please).<br>
- RST Connect to +3v3 using a 4k7->10k resistor.
- scl, sclk -> SCLK
- sda, sdin -> MOSI
- DC,RS ->DC pin on MCU
- some oled has WR, RD. Leave floating...
- some oled has D0...D7. Most of the times D0 and D1 are used for SPI, connect D2...D7 to GND or will be unstable!
- VIN. Most oled support 5v, internally they have regulators, but follow specifications of your particular oled!
- 3v3. This is 99% of the times an OUT! So do not use it.<br>

***
<b>Some notes, please read this, will save your time:</b><br>
- Even if your OLED can work at 5V, it cannot accept logic levels at 5V!!! All OLED accept logic levels at 3v3 so if you plan to use an AVR (like UNO) you need a level converted (like CD4050 or HCF4050 powered at 3v3, very cheap).
- Do not use logic converters based on mosfet or similar or bidirectional ones (Adafruit and Sparkfun sell a lot of these), they are crap, waveforms are distorted and DO NOT USE PARTITION RESISTOR's for that, it's a bad idea, you can damage oled and cpu in the same time.
- Most of you know already this but remember that Arduino UNO or similar can provide 3v3 but the logic levels are always at 5V!!!
- The RST pin is not strictly necessary but if not used must pullup to 3v3, never leave float! When is necessary? When you develop a library for example but if you change libraries you may need a complete power cycle if you are not using this pin.
- ESP8266 has very weak SPI, should be 3v3 but most of the times is much less so better avoid use any logic chip between it and oled, I spend days around a circuit before discovering this.
- On ESP8266, from April 2016 the pin GPIO16 misteriously doesn't works as CS, don't ask me why. I'm currently using GPIO2 or GPIO4.
- Since I'm using the fast SPI possible, keep wires short and remember to provide a decoupling capacitor for your oled.<br>
- If you plan to use SPI for other devices as well it's a good idea pullup the CS with a 10K resistor to +3v3, this will keep oled disabled until your CPU access it for initialization and avoid interferences.
- It's always a good idea provide a pullup for each CS if multiple SPI devices are used, when CPU start all devices are forced disabled and CPU is able to access one by one and initialize all of them correctly, keep in mind and you will happy in the future!<br>

***

<b>Performances (SSD1331):</b><br>
Early alpha was not working, only garbage on screen, it's here for just for reference<br>
Alpha 1..2 was using absolute screen addressing (now abandoned)<br>
Alpha 3..4 and beta's use hardware rotation addressing. (In Alpha 4 triangles rendering failed)<br>
Beta 1..2 works and are usable, all function tested at any rotation.<br>
CPU used for benchmarks: SSD1331, Teensy 3.2 at 96Mhz. Sketch:benchmarks_official.ino<br>
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

***
<b>Licence:</b><br>
Licensed under GNU V3.
 
