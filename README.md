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

<b>CPU Supported:</b><br>
 - Teensy 3.0, 3.1, 3.2, 3.4, 3.5, LC
 - Teensy 2.0 and variant, any Arduino 8 bit
 - Arduino DUE
 - ESP8266
 - SPARK (next versions)
 - Several ST (next versions)<br>

<b>Note:</b> I will not support Arduino Zero, it cost a fortune, I suggest a Teensy that is much cheaper and more featured!<br>
However, this is open source an released with GNU V3 so you can fork library and add Zero support yourself as you respect the GNU licence and leave intact any comment.<br>

<b>Background:</b><br>
I got my first SSD_1331 on Ebay, find a library compatible at Adafruit but I was extremely disappointed about performances/features and contains lot of bugs, so I decided to write a new one from scratch based on my popular TFT_ILI9163C.<br>
Unfortunatly these devices are similar each others but commands and methods can be really different, in particular SSD_1331/1332 uses commands to send data when most chip uses dataTransfer.<br>
I have 2 old libraries here on github, but digging inside datasheet I discover that I can get much more by rewrote almost everithing. As result I got all chip features enabled and the currently faster driver, much more than adafruit and works with all features at any screen rotation by using all the time his hardware accellerated primitives<br>
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

<b>Unique commands: (differs from other libraries due different chip features)</b><br>
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


<b>Licence:</b><br>
Licenced under GNU V3.
 
