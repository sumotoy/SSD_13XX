# SSD_13XX
A very fast and full featured driver for SOLOMON SYSTECH SSD1331, SSD1332 and SSD1351 OLED compatible with Teensy's and many other MCU's.
I have other 2 libraries here in github but have some issues and are quite old, this will replace both.<br>
<b>NOTE: Currently in development, not all files upped! Please wait this note disappear before download.</b><br>

<b>Driver supported so far:</b><br>
  - SSD_1331: Hardware accellerated
  - SSD_1332: Hardware accellerated
  - SSD_1351: Hardware accellerated (1.27") (1.5")<br>

<b>Driver that will never supported:</b><br>
 - SSD_1306: This device need a completely different library since it have no RAM.<br>

<b>Background:</b><br>
I got my first SSD_1331 on Ebay, find a library compatible at Adafruit but it's really slow, poor featured and uses just one accellerated primitive and has several issues so I decided to write a new one from scratch.<br>
Unfortunatly these devices are similar each others but commands and methods can be really different, in particular SSD_1331/1332 lack of some important registers and it's tricky add rotation support.<br>
I have 2 old libraries here on github, both old and still uses some Adafruit stuff but I have decided to get rid of all code and use much faster approach, the speed is really impressive compared to old stuff.

<b>Features:</b><br>
 - Extreme fast, very chip hardware limit.
 - Easy to add display's.
 - Uses all chip Hardware accellerated routines.
 - Standard SPI & SPI transaction compatible, play nice with other SPI devices.
 - Many CPU supported.
 - Ultrafast Teensy's 3.0,3.1,3.2,3.4,3.5 SPI FIFO.
 - Almost all chip features supported.
 - LGPO Font accellerated rendering.
 - Icon support.
 - Image support.
 - Gradients.
 - Uses my TFT_9163C popular library format and commands, can share fonts,icons and images.
