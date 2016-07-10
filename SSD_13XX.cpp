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
===========================================================================================*/

#include "SSD_13XX.h"


/*********************************************************
********************** constructors **********************
**********************************************************/


#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	SSD_13XX::SSD_13XX(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin,const uint8_t mosi,const uint8_t sclk)
	{
		_cs   = cspin;
		_dc   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
	}
#elif defined(__MKL26Z64__) //Teensy LC
	SSD_13XX::SSD_13XX(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin,const uint8_t mosi,const uint8_t sclk)
	{
		_cs   = cspin;
		_dc   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
		_useSPI1 = false;
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) _useSPI1 = true;
	}
#else //All the rest
	SSD_13XX::SSD_13XX(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin)
	{
		_cs   = cspin;
		_dc   = dcpin;
		_rst  = rstpin;
	}
#endif



/*********************************************************
*******************    some SPI stuff     ****************
**********************************************************/
#if defined(__AVR__)
//-----------------Arduino Uno, Leonardo, Mega, Teensy 2.0, any 8 bit AVR
	#if !defined (SPI_HAS_TRANSACTION)
	void SSD_13XX::setBitrate(uint32_t n)
	{
		if (n >= 8000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV2);
		} else if (n >= 4000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV4);
		} else if (n >= 2000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV8);
		} else {
			SPI.setClockDivider(SPI_CLOCK_DIV16);
		}
	}
	#endif
#elif defined(__SAM3X8E__)
//------------------------------------------Arduino Due
	#if !defined(SPI_HAS_TRANSACTION)
	void SSD_13XX::setBitrate(uint32_t n)
	{
		uint32_t divider = 1;
		while (divider < 255) {
			if (n >= 84000000 / divider) break;
			divider = divider - 1;
		}
		SPI.setClockDivider(divider);
	}
	#endif
#elif defined(__MKL26Z64__)
//-----------------------------------------Teensy LC
	#if !defined (SPI_HAS_TRANSACTION)
	void SSD_13XX::setBitrate(uint32_t n)
	{
		//nop
	}
	#endif
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
//-----------------------------------------Teensy 3.0 & 3.1 & 3.2
	#if !defined (SPI_HAS_TRANSACTION)
	void SSD_13XX::setBitrate(uint32_t n)
	{
		//nop
	}
	#endif
#else
	#if !defined (SPI_HAS_TRANSACTION)
	void SSD_13XX::setBitrate(uint32_t n)
	{
		if (n >= 8000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV2);
		} else if (n >= 4000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV4);
		} else if (n >= 2000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV8);
		} else {
			SPI.setClockDivider(SPI_CLOCK_DIV16);
		}
	}
	#endif
#endif

/*********************************************************
************** Var init and SPI preparation **************
**********************************************************/
void SSD_13XX::begin(bool avoidSPIinit)
{
//initialize Vars
	_remapReg		= 0;
	_currentMode	= 0b00000000;
	_portrait 		= false;
	_filled 		= 0;//set like this to force it change
	_initError 		= 0b00000000;
	_cursorY  		= _cursorX 		= 0;
	_textScaleX 	= _textScaleY 	= 1;
	_centerText 	= false;
	_fontInterline 	= 0;
	_charSpacing 	= 0;
	_textWrap      	= true;
	setColorDepth(SSD_COLORDEPTH);
	setColorOrder(SSD_RGBORDER);
	_defaultBgColor = _SSD_DEF_BACKGROUND;
	_defaultFgColor = _SSD_DEF_FOREGROUND;
	_textForeground = _textBackground = _defaultFgColor;//text transparent
	_arcAngleMax 	= 360;
	_arcAngleOffset = -90;
	#if defined(SPI_HAS_TRANSACTION)
		SSD_13XXSPI = SPISettings(_common_max_SPI_speed, MSBFIRST, SPI_MODE0);
	#endif
#if defined(__AVR__)//(avr) Any 8Bit AVR
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = portOutputRegister(digitalPinToPort(_cs));
	rsport    = portOutputRegister(digitalPinToPort(_dc));
	cspinmask = digitalPinToBitMask(_cs);
	dcpinmask = digitalPinToBitMask(_dc);
    if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (!avoidSPIinit){
			SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	*csport |= cspinmask;//hi
	enableDataStream();
#elif defined(__SAM3X8E__)//(arm) DUE
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = digitalPinToPort(_cs);
	rsport    = digitalPinToPort(_dc);
	cspinmask = digitalPinToBitMask(_cs);
	dcpinmask = digitalPinToBitMask(_dc);
    if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (!avoidSPIinit){
			SPI.setClockDivider(5); // 8 MHz
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	csport->PIO_SODR  |=  cspinmask;//HI
	enableDataStream();
#elif defined(__MKL26Z64__)//(arm) Teensy LC (preliminary)
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (_useSPI1){
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) {//identify alternate SPI channel 1 (24Mhz)
			SPI1.setMOSI(_mosi);
			SPI1.setSCK(_sclk);
			if (!avoidSPIinit) SPI1.begin();
			_useSPI1 = true; //confirm
		} else {
			bitSet(_initError,0);
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {//ERROR
			bitSet(_initError,1);
			return;
		}
	} else {
		if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {//valid SPI pins?
			SPI.setMOSI(_mosi);
			SPI.setSCK(_sclk);
			if (!avoidSPIinit) SPI.begin();
			_useSPI1 = false; //confirm
		} else {
			bitSet(_initError,0);
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {//ERROR
			bitSet(_initError,1);
			return;
		}
	}
	#if defined(_TEENSYLC_FASTPORT)
		csportSet    	= portSetRegister(digitalPinToPort(_cs));
		csportClear     = portClearRegister(digitalPinToPort(_cs));
		cspinmask 		= digitalPinToBitMask(_cs);
		dcportSet       = portSetRegister(digitalPinToPort(_dc));
		dcportClear     = portClearRegister(digitalPinToPort(_dc));
		dcpinmask	    = digitalPinToBitMask(_dc);
		*csportSet 		= cspinmask;
	#else
		digitalWriteFast(_cs,HIGH);
	#endif
		enableDataStream();
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {
        SPI.setMOSI(_mosi);
        SPI.setSCK(_sclk);
	} else {
		bitSet(_initError,0);
		return;
	}
	if (!avoidSPIinit) SPI.begin();
	if (SPI.pinIsChipSelect(_cs, _dc)) {
		pcs_data = SPI.setCS(_cs);
		pcs_command = pcs_data | SPI.setCS(_dc);
	} else {
		pcs_data = 0;
		pcs_command = 0;
		bitSet(_initError,1);
		return;
	}
#elif defined(ESP8266)//(arm) XTENSA ESP8266
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (!avoidSPIinit){
			SPI.setClockDivider(4);
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	#if defined(ESP8266) && defined(_ESP8266_STANDARDMODE)
		digitalWrite(_cs,HIGH);
	#else
		GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_cs));//H
	#endif
	enableDataStream();
#else//(xxx) Rest of CPU
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (!avoidSPIinit) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (!avoidSPIinit){
			SPI.setClockDivider(4);
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	digitalWrite(_cs,HIGH);
	enableDataStream();
#endif
	if (_rst != 255) {
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delay(10);
		digitalWrite(_rst, LOW);
		delay(10);
		digitalWrite(_rst, HIGH);
		delay(10);
	}
	/* -----------------------------------------------------------
	------------------- Chip Initialization ----------------------
	-------------------------------------------------------------*/
	delay(30);
	startTransaction();
	#if defined(_SSD_1331_96X64_H) || defined(_SSD_1332_96X64_H)
		if (SSD_COMSPLIT == 1){
			_remapReg |= ((1 << 5));
		} else {
			_remapReg |= ((0 << 5));
		}
		writecommand_cont(CMD_DISPLAYOFF);
		setRegister_cont(CMD_CLOCKDIV,SSD_CLOCKDIV);
		setRegister_cont(CMD_SETMULTIPLEX,SSD_SETMULTIPLEX);
		setRegister_cont(CMD_STARTLINE,SSD_STARTLINE);
		setRegister_cont(CMD_DISPLAYOFFSET,SSD_DISPLAYOFFSET);
		setRegister_cont(CMD_PHASEPERIOD,SSD_PHASEPERIOD);
		setRegister_cont(CMD_SETMASTER,SSD_SETMASTER);
		setRegister_cont(CMD_POWERMODE,SSD_POWERMODE);
		setRegister_cont(CMD_MASTERCURRENT,SSD_MASTERCURRENT);
		#if defined(SSD_GAMMASET)
			writecommand_cont(CMD_GRAYSCALE); for (uint8_t i =0;i<32;i++){writecommand_cont(SSD_GRAYTABLE[i]);}
		#else
			writecommand_cont(CMD_LINEARGRAY);
		#endif
		setRegister_cont(CMD_CONTRASTA,SSD_CONTRAST_A);
		setRegister_cont(CMD_CONTRASTB,SSD_CONTRAST_B);
		setRegister_cont(CMD_CONTRASTC,SSD_CONTRAST_C);
		setRegister_cont(CMD_VPACOLORLVL,SSD_VPACOLORLVL);
		setRegister_cont(CMD_VPBCOLORLVL,SSD_VPBCOLORLVL);
		setRegister_cont(CMD_VPCCOLORLVL,SSD_VPCCOLORLVL);
		#if defined(_SSD_1331_96X64_H)
			writecommand_cont(CMD_DIMMODESET);
			writecommand_cont(0);
			writecommand_cont(SSD_DIMMDESET_A);
			writecommand_cont(SSD_DIMMDESET_B);
			writecommand_cont(SSD_DIMMDESET_C);
			writecommand_cont(SSD_DIMMDESET_PC);
			setRegister_cont(CMD_PRECHARGE,SSD_PRECHARGE);
			setRegister_cont(CMD_PRECHARGEA,SSD_PRECHARGE_A);
			setRegister_cont(CMD_PRECHARGEB,SSD_PRECHARGE_B);
			setRegister_cont(CMD_PRECHARGEC,SSD_PRECHARGE_C);
			setRegister_cont(CMD_PRECHARGELEVEL,SSD_PRECHARGELEVEL);
		#endif
		setRegister_cont(CMD_VCOMH,SSD_VCOMH);
		_fillUtility(1);
	#elif defined(_SSD_1351_96X64_H) || defined(_SSD_1351_128X96_H) || defined(_SSD_1351_128X128_H)
		if (SSD_COMSPLIT == 1){
			_remapReg |= ((1 << 5));
		} else {
			_remapReg |= ((0 << 5));
		}
		setRegister_cont(CMD_CMDLOCK,SSD_COMMANDLOCK1);
		setRegister_cont(CMD_CMDLOCK,SSD_COMMANDLOCK2);
		writecommand_cont(CMD_DISPLAYOFF);
		setRegister_cont(CMD_CLOCKDIV,SSD_CLOCKDIV);
		setRegister_cont(CMD_MUXRATIO,SSD_MUXRATIO);
		setRegister_cont(CMD_STARTLINE,SSD_STARTLINE);
		setRegister_cont(CMD_DISPLAYOFFSET,SSD_DISPLAYOFFSET);
		setRegister_cont(CMD_SETGPIO,SSD_SETGPIO);
		setRegister_cont(CMD_FUNCTIONSELECT,SSD_FUNCTIONSELECT);
		writecommand_cont(CMD_SETVSL);
		writedata8_cont(SSD_SETVSL_A);writedata8_cont(SSD_SETVSL_B);writedata8_cont(SSD_SETVSL_C);
		writecommand_cont(CMD_CONTRASTABC);
		writedata8_cont(SSD_CONTRAST_A);writedata8_cont(SSD_CONTRAST_B);writedata8_cont(SSD_CONTRAST_C);
		setRegister_cont(CMD_MASTERCURRENT,SSD_MASTERCURRENT);
		writecommand_cont(CMD_DISPLAYENHANCE);
		if (SSD_ENHANCE){
			writedata8_cont(0xA4);
		} else {
			writedata8_cont(0x00);
		}
		writedata8_cont(0x00);
		writedata8_cont(0x00);
		#if defined(SSD_GAMMASET)
			//writecommand_cont(CMD_GRAYSCALE); for (uint8_t i =0;i<32;i++){writedata8_cont(SSD_GRAYTABLE[i]);}
		#else
			writecommand_cont(CMD_USELUT);
		#endif
		// phase here
		setRegister_cont(CMD_PRECHARGE,SSD_PRECHARGE);
		setRegister_cont(CMD_PRECHARGE2,SSD_PRECHARGE2);
		setRegister_cont(CMD_VCOMH,SSD_VCOMH);
	#endif
	//setAddrWindow_cont(0,0,SSD_WIDTH-1,SSD_HEIGHT-1,false);// ???
	//_pushColors_cont(_defaultBgColor, SSD_CGRAM);//???
	//Normal Display and turn ON
	writecommand_cont(CMD_NORMALDISPLAY);
	writecommand_last(CMD_DISPLAYON);
	endTransaction();
	delay(60);
	setRotation(_SSD_DEF_ROTATION);
	fillScreen(_defaultBgColor);
	//Now set Font
	#if defined(_SSD_DEF_FONT_PATH)
		setFont(&_SSD_DEF_FONT_NAME);
	#else
		setFont(&nullfont);
	#endif
	delay(1);
	changeMode(NORMAL);
}

void SSD_13XX::setRegister_cont(const uint8_t cmd,uint8_t data)
{
	writecommand_cont(cmd);
	#if defined(_SSD_USECMDASDATA)
		writecommand_cont(data);
	#else
		writedata8_cont(data);
	#endif
}

/*********************************************************
***************** Basic display commands *****************
**********************************************************/
/*
A simple helper to detect some error in initialization.
Since this library doesn't use MISO it can detect only
very basic errors:
0b00000001: Wrong MOSI,SCLK pin (only for some MCU's)
0b00000010: Wrong CS pin (only for some MCU's)
0b10000000: Display NOT supported (Only for multi-instances)
*/
uint8_t SSD_13XX::getErrorCode(void)
{
	return _initError;
}

/*
This change the mode of the display as:
	NORMAL: Normal mode.
	PWRSAVE: Consume much less power
	PROTECT: Display protect his serial comm, accept only a change mode as normal to exit protected state
	INVERT: It invert the display
	DISP_ON: Turn on display (if off) and enable backlight pin if used
	DISP_DIM: The all display goe dim
	DISP_OFF: The opposite of above
*/
void SSD_13XX::changeMode(const enum SSD_13XX_modes m)
{
	if (m != _currentMode){
		startTransaction();
		switch(m){
			case NORMAL:
				if (_currentMode == 6) {//was in off display?
					writecommand_cont(CMD_DISPLAYON);
				}
				/*
				if (_currentMode == 2) {//was in idle?
					//writecommand_cont(CMD_IDLEOF);
				}
				*/
				
				#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
				if (_currentMode == 8) {//was in powerMode?
					setRegister_cont(CMD_POWERMODE,SSD_POWERMODE);
					//delay(120);//needed
				}
				#endif
				
				if (_currentMode == 4){//was inverted?
					//SSD1332 should need only CMD_NORMALDISPLAY!?!
					#if defined(_SSD_1331_REG_H_)
					writecommand_cont(CMD_DINVOF);
					#endif
				}
				#if defined(_SSD_1331_REG_H_) || defined(_SSD_1351_REG_H_)
				if (_currentMode == 9){//was in protect mode?
					setRegister_cont(CMD_CMDLOCK,0x12);//unlock
				}
				#endif
				/*
				if (_currentMode == 12 || _currentMode == 13){//all on or off?
					//nothing
				}
				*/
				writecommand_cont(CMD_NORMALDISPLAY);
				_currentMode = 0;
			break;
			case ALL_ON:
				writecommand_cont(CMD_DISPLAYALLON);
				_currentMode = 12;
			break;
			case ALL_OFF:
				writecommand_cont(CMD_DISPLAYALLOFF);
				_currentMode = 13;
			break;
			case PWRSAVE: //power mode ON
				#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
					writecommand_cont(CMD_POWERMODE);
					writecommand_last(0x1A);
					_currentMode = 8;
					delay(5);//needed
				#else
					//TODO: exist?
					//writedata8_last(0x1A);
				#endif
				endTransaction();
				return;
			case INVERT:
				writecommand_cont(CMD_INVERTDISPLAY);//OK
				_currentMode = 4;
			break;
			case DISP_ON:
				writecommand_cont(CMD_DISPLAYON);
				_currentMode = 5;
			break;
			case DISP_OFF:
				writecommand_cont(CMD_DISPLAYOFF);
				_currentMode = 6;
			break;
			/*
			case DISP_DIM:
				writecommand_cont(CMD_DISPLAYDIM);
				_currentMode = 7;
			break;
			*/
			case PROTECT:
				#if defined(_SSD_1331_REG_H_) || defined(_SSD_1351_REG_H_)
				setRegister_cont(CMD_CMDLOCK,0x16);//lock
				_currentMode = 9;
				#else
					endTransaction();
					return;
				#endif
			break;
			default:
				endTransaction();
				return;
			break;
		}
		closeTransaction();
	}
}

uint8_t SSD_13XX::getMode(void)
{
	return _currentMode;
}


void SSD_13XX::setBrightness(uint8_t brightness)
{
	
	if (brightness > 15) brightness = 15;
	startTransaction();
		writecommand_cont(CMD_MASTERCURRENT);
		#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
			writecommand_last(brightness);
		#else
			writedata8_last(brightness);
		#endif
	endTransaction();
}

void SSD_13XX::setArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1,true);
	closeTransaction();
}


void SSD_13XX::copyArea(int16_t sx0, int16_t sy0, int16_t sx1, int16_t sy1,int16_t dx, int16_t dy)
{
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
	if (_portrait){//not tested yet
		swapVals(sx0,sy0);
		swapVals(sx1,sy1);
		swapVals(dx,dy);
	}
	startTransaction();
	writecommand_cont(CMD_DRAWCOPY);
		writecommand_cont(sx0 & 0xFF);
		writecommand_cont(sy0 & 0xFF);
		writecommand_cont(sx1 & 0xFF);
		writecommand_cont(sy1 & 0xFF);
		writecommand_cont(dx & 0xFF);
		writecommand_last(dy & 0xFF);
	endTransaction();
	#endif
}

void SSD_13XX::dimArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
	if (_portrait){//not tested yet
		swapVals(x0,y0);
		swapVals(x1,y1);
	}
	startTransaction();
	writecommand_cont(CMD_DIMWINDOW);
		writecommand_cont(x0 & 0xFF);
		writecommand_cont(y0 & 0xFF);
		writecommand_cont(x1 & 0xFF);
		writecommand_last(y1 & 0xFF);
	endTransaction();
	#endif
}

void SSD_13XX::moveArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
	if (_portrait){//not tested yet
		swapVals(x0,y0);
		swapVals(x1,y1);
	}
	startTransaction();
	writecommand_cont(CMD_CLRWINDOW);
		writecommand_cont(x0 & 0xFF);
		writecommand_cont(y0 & 0xFF);
		writecommand_cont(x1 & 0xFF);
		writecommand_last(y1 & 0xFF);
	endTransaction();
	#endif
}

uint8_t SSD_13XX::getRotation(void)
{
	return _rotation;
}

void SSD_13XX::setColorDepth(uint8_t depth)
{
	if (depth == 16){
		_colorDepth = 16;
		_remapReg |= ((0 << 7) | (1 << 6));
	#if defined(_SSD_1351_REG_H_)
	} else if (depth == 18) {
		_colorDepth = 16;
		_remapReg |= ((1 << 7) | (0 << 6));
	#endif
	} else {
		_colorDepth = 8;
		_remapReg |= ((0 << 7) | (0 << 6));
	}
}

void SSD_13XX::setColorOrder(bool order)
{
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1351_REG_H_)
		_remapReg |= ((order << 2));
	#endif
}

//+++++++++OK
void SSD_13XX::setRotation(uint8_t m)
{
	_rotation = m % 4; // can't be higher than 3
	_portrait = false;
	_remapReg &= ~(0x1B);//clear bit 0,1,3,4
	_width  = SSD_WIDTH;
	_height = SSD_HEIGHT;
	if (_rotation == 0){
		#if defined(_SSD_1331_REG_H_)
			_remapReg |= ((1 << 4) | (1 << 1));//bit 4 & 1
		#elif defined(_SSD_1332_REG_H_)
			_remapReg |= ((1 << 4));//bit 4
		#elif defined(_SSD_1351_REG_H_)
			_remapReg |= ((1 << 4));//(1)
		#else
			//TODO
		#endif
	} else if (_rotation == 1){
		#if defined(_SSD_1331_REG_H_)
			_remapReg |= ((1 << 4) | (1 << 0));//bit 4 & 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(_SSD_1332_REG_H_)
			_remapReg |= ((1 << 4) | (1 << 1) | (1 << 0));//bit 4 & 1 & 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(_SSD_1351_REG_H_)
			_remapReg |= ((1 << 4) | (1 << 1) | (1 << 0));//(2)
			swapVals(_width,_height);
			_portrait = true;
		#else
			//TODO
		#endif
	} else if (_rotation == 2){
		#if defined(_SSD_1331_REG_H_)
		#elif defined(_SSD_1332_REG_H_)
			_remapReg |= ((1 << 1));//bit 1
		#elif defined(_SSD_1351_REG_H_)
			_remapReg |= ((1 << 1));//(3)
		#else
			//TODO
		#endif
	} else {
		#if defined(_SSD_1331_REG_H_)
			_remapReg |= ((1 << 1) | (1 << 0));//bit 1 & 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(_SSD_1332_REG_H_)
			_remapReg |= ((1 << 0));//bit 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(_SSD_1351_REG_H_)
			_remapReg |= ((1 << 0));//(0)
			swapVals(_width,_height);
			_portrait = true;
		#else
			//TODO
		#endif
	}
	_cursorX = 0; _cursorY = 0;//always reset cursor
	startTransaction();
		setAddrWindow_cont(0,0,_height-1,_width-1,false);
		writecommand_cont(CMD_SETREMAP);//set remap
		#if defined(_SSD_USECMDASDATA)
			writecommand_last(_remapReg);
		#else
			writedata8_last(_remapReg);
		#endif
	endTransaction();
}

//+++++++++OK
int16_t SSD_13XX::width(void) const {
	return _width;
}

//+++++++++OK
int16_t SSD_13XX::height(void) const {
	return _height;
}

//+++++++++OK
int16_t SSD_13XX::cgWidth(void) const {
	return SSD_WIDTH;
}

//+++++++++OK
int16_t SSD_13XX::cgHeight(void) const {
	return SSD_HEIGHT;
}


/*********************************************************
************************   scroll  ***********************
**********************************************************/

/*
a:Set number of column as horizontal scroll offset Range: 0d-95d ( no horizontal scroll if equals to 0)
b:Define start row address
c:Set number of rows to be horizontal scrolled B[5:0]+C[6:0] <=64
d:Set number of row as vertical scroll offset Range: 0d-63d ( no vertical scroll if equals to 0)
e:Set time interval between each scroll step
*/
void SSD_13XX::defineScrollArea(int16_t a, int16_t b, int16_t c, int16_t d, uint8_t e)
{
	if (b+c > SSD_HEIGHT) return;
	uint8_t spd = 0;
	e = e % 4;
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
		if (e == 0){
			spd = 0b00000000;
		} else if (e == 1){
			spd = 0b00000001;
		} else if (e == 2){
			spd = 0b00000010;
		} else {
			spd = 0b00000011;
		}
		startTransaction();
		writecommand_cont(CMD_SCROLL_SET);

		writecommand_cont(a & 0xFF);
		writecommand_cont(b & 0xFF);
		writecommand_cont(c & 0xFF);
		writecommand_cont(d & 0xFF);
		writecommand_last(spd);
		endTransaction();
	#elif defined(_SSD_1351_REG_H_)
		if (e == 1){
			spd |= ((1 << 0));
		} else if (e == 2){
			spd |= ((1 << 1));
		} else if (e > 2) {
			spd |= ((1 << 1) | (1 << 0));
		}
		startTransaction();
		writecommand_cont(CMD_HORIZSCROLL);
		writedata8_cont(0b00000001);//0b10000001
		writedata8_cont(b & 0xFF);
		writedata8_cont(c & 0xFF);
		writedata8_cont(0);
		writedata8_last(spd);
		endTransaction();
	#endif
	
}


boolean SSD_13XX::scroll(bool active)
{
	startTransaction();
	if (active){
		writecommand_last(CMD_SCROLL_ON);
	} else {
		writecommand_last(CMD_SCROLL_OFF);
	}
	endTransaction();
	return active;
}


/*********************************************************
******************** Color Functions *********************
**********************************************************/

/**************************************************************************/
/*!
	  calculate a gradient color
	  return a spectrum starting from blue to red (0...127)
	  From my RA8875 library
*/
/**************************************************************************/
//+++++++++OK
uint16_t SSD_13XX::gradient(uint8_t val)
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t q = val / 32;
	switch(q){
		case 0: r = 0; g = 2 * (val % 32); b = 31; break;
		case 1: r = 0; g = 63; b = 31 - (val % 32); break;
		case 2: r = val % 32; g = 63; b = 0; break;
		case 3: r = 31; g = 63 - 2 * (val % 32); b = 0; break;
	}
	return (r << 11) + (g << 5) + b;
}

/**************************************************************************/
/*!
	  interpolate 2 16bit colors
	  return a 16bit mixed color between the two
	  Parameters:
	  color1:
	  color2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
	  NOTE: Needs a remake, it's slow! (TODO)
*/
/**************************************************************************/
//+++++++++OK
uint16_t SSD_13XX::colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return color1;
    if (pos >= div) return color2;
	uint8_t r1,g1,b1;
	Color565ToRGB(color1,r1,g1,b1);//split in r,g,b
	uint8_t r2,g2,b2;
	Color565ToRGB(color2,r2,g2,b2);//split in r,g,b
	return colorInterpolation(r1,g1,b1,r2,g2,b2,pos,div);
}

/**************************************************************************/
/*!
	  interpolate 2 r,g,b colors
	  return a 16bit mixed color between the two
	  Parameters:
	  r1.
	  g1:
	  b1:
	  r2:
	  g2:
	  b2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
	  NOTE: Needs a remake, it's slow! (TODO)
*/
/**************************************************************************/
//+++++++++OK
uint16_t SSD_13XX::colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return Color565(r1,g1,b1);
    if (pos >= div) return Color565(r2,g2,b2);
	float pos2 = (float)pos/div;
	return Color565(
				(uint8_t)(((1.0 - pos2) * r1) + (pos2 * r2)),
				(uint8_t)(((1.0 - pos2) * g1) + (pos2 * g2)),
				(uint8_t)(((1.0 - pos2) * b1) + (pos2 * b2))
	);
}

//+++++++++OK
void SSD_13XX::setBackground(uint16_t color)
{
	_defaultBgColor = color;
}

//+++++++++OK
void SSD_13XX::setForeground(uint16_t color)
{
	_defaultFgColor = color;
}

//+++++++++OK
uint16_t SSD_13XX::getBackground(void)
{
	return _defaultBgColor;
}

//+++++++++OK
uint16_t SSD_13XX::getForeground(void)
{
	return _defaultFgColor;
}

/*********************************************************
****************** Graphic Functions *********************
**********************************************************/
//+++++++++OK
void SSD_13XX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	//if (boundaryCheck(x,y)) return;
	//if ((x < 0) || (y < 0)) return;
	startTransaction();
	drawPixel_cont(x,y,color);
	closeTransaction();
}


//+++++++++OK
void SSD_13XX::fillScreen(uint16_t color)
{
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
		uint8_t r1,g1,b1;
		_convertColor(color,r1,g1,b1);
		startTransaction();
		_fillUtility(1);
		writecommand_cont(CMD_DRAWRECT);
		writecommand16_cont(0);
		writecommand_cont(SSD_WIDTH-1);
		writecommand_cont(SSD_HEIGHT-1);
		_sendColor_cont(r1,g1,b1);
		_sendColor_cont(r1,g1,b1);
		delayMicroseconds(CMD_DLY_FILL);//CMD_DLY_FILL
	#else
		startTransaction();
		setAddrWindow_cont(
				0,
				0,
				SSD_WIDTH - 1,
				SSD_HEIGHT - 1,
				false
		);
		_pushColors_cont(color, SSD_CGRAM);
	#endif
	closeTransaction();
}



//with gradient
//+++++++++OK
void SSD_13XX::fillScreen(uint16_t color1,uint16_t color2)
{
	startTransaction();
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
	if (color1 != color2){
		fillRect_cont(0,0,SSD_WIDTH,SSD_HEIGHT,color1,color2);
	} else {
		uint8_t r1,g1,b1;
		_convertColor(color1,r1,g1,b1);
		_fillUtility(1);
		writecommand_cont(CMD_DRAWRECT);
		writecommand16_cont(0);
		writecommand_cont(SSD_WIDTH-1);
		writecommand_cont(SSD_HEIGHT-1);
		_sendColor_cont(r1,g1,b1);
		_sendColor_cont(r1,g1,b1);
		delayMicroseconds(CMD_DLY_FILL);
	}
	#else
		if (color1 != color2){
			fillRect_cont(0,0,SSD_WIDTH,SSD_HEIGHT,color1,color2);
		} else {
			setAddrWindow_cont(
				0,
				0,
				SSD_WIDTH - 1,
				SSD_HEIGHT - 1,
				false
			);
			_pushColors_cont(color1, SSD_CGRAM);
		}
	#endif
	closeTransaction();
}

//+++++++++OK
void SSD_13XX::clearScreen(void)
{
	fillScreen(_defaultBgColor);
	_cursorX = _cursorY = 0;
}

/*
draw fast vertical line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void SSD_13XX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	/*
	if (boundaryCheck(x,y)) return;
	if (((y + h) - 1) >= _height) h = _height - y;
	h = sizeCheck(y,h,_height);
	*/
	if (x >= _width-1) return;
	if (y >= _height-1) return;
	startTransaction();
	drawFastVLine_cont(x,y,h,color);
	closeTransaction();
}


/*
draw fast horizontal line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void SSD_13XX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	/*
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width - x;
	w = sizeCheck(x,w,_width);
	*/
	if (x >= _width-1) return;
	if (y >= _height-1) return;
	startTransaction();
	drawFastHLine_cont(x,y,w,color);
	closeTransaction();
}


/*
fill RECT
*/
//+++++++++OK
void SSD_13XX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	startTransaction();
	drawRect_cont(x, y, w, h, color,color,true);
	closeTransaction();
}

/*
fill RECT with gradient
*/
void SSD_13XX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2)
{
	if (boundaryCheck(x,y)) return;
	startTransaction();
	fillRect_cont(x,y,w,h,color1,color2);
	closeTransaction();
}


//Updated, new way is fast!!!
void SSD_13XX::fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2)
{
	//uint16_t colorTemp;
	if (w < 2 && h < 2){ drawPixel_cont(x,y,color1); return; }
	if (h < 2) { drawFastHLine_cont(x,y,w,color1); return; }
	if (w < 2) { drawFastVLine_cont(x,y,h,color1); return; }
	setAddrWindow_cont(x,y,(x+w)-1,(y+h)-1,true);
	if (color1 != color2){
		uint16_t pos = 0;
		uint8_t r1,r2,g1,g2,b1,b2;
		Color565ToRGB(color1,r1,g1,b1);
		Color565ToRGB(color2,r2,g2,b2);
		float pos2;
		uint8_t rR,gG,bB;
		uint16_t tot = h;
		uint16_t wtemp = w;
		do {
			pos2 = (float)pos/tot;
			rR = (((1.0 - pos2) * r1) + (pos2 * r2));
			gG = (((1.0 - pos2) * g1) + (pos2 * g2));
			bB = (((1.0 - pos2) * b1) + (pos2 * b2));
			#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
				do {
					writedata16_cont(Color565(rR,gG,bB));
				} while (--wtemp > 0);
				wtemp = w;
			#else
				_pushColors_cont(Color565(rR,gG,bB),wtemp);
			#endif
			pos++;
		} while (--h > 0);
	} else {
		_pushColors_cont(color1,w*h);
	}
}


/*
draw LINE
*/
void SSD_13XX::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
	if (x1 >= _width) x1 = _width-1;
	if (y1 >= _height) y1 = _height-1;
	startTransaction();
	drawLine_cont(x0,y0,x1,y1,color/*,200*/);
	closeTransaction();
}

/*
draw rect
*/

//+++++++++OK
void SSD_13XX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	drawRect(x,y,w,h,color,color,false);
}

//+++++++++OK
void SSD_13XX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2,bool filled)
{
	if (x + w >= _width) return;
	if (y + h >= _height) return;
	startTransaction();
	drawRect_cont(x, y, w, h, color1,color2,filled);
	closeTransaction();
}


void SSD_13XX::drawArcHelper(int16_t cx, int16_t cy, int16_t radius, int16_t thickness, float start, float end, uint16_t color)
{
	if (cx + radius > _width) return;
	if (cy + radius > _height) return;
	int16_t xmin = 65535;
	int16_t xmax = -32767;
	int16_t ymin = 32767;
	int16_t ymax = -32767;
	float cosStart, sinStart, cosEnd, sinEnd;
	float r, t;
	float startAngle, endAngle;
	int16_t ir2, or2;//int
	int16_t x, y, x2, y2;//int
	int16_t y1s, y1e, y2s;//int
	float sslope, eslope;
	bool y1StartFound, y2StartFound, y1EndFound, y2EndSearching;

	startAngle = (start / _arcAngleMax) * 360;	// 252
	endAngle = (end / _arcAngleMax) * 360;		// 807

	while (startAngle < 0) startAngle += 360;
	while (endAngle < 0) endAngle += 360;
	while (startAngle > 360) startAngle -= 360;
	while (endAngle > 360) endAngle -= 360;

	if (startAngle > endAngle) {
		drawArcHelper(cx, cy, radius, thickness, ((startAngle / 360.0) * _arcAngleMax), _arcAngleMax, color);
		drawArcHelper(cx, cy, radius, thickness, 0, ((endAngle / 360.0) * _arcAngleMax), color);
	} else {
		// Calculate bounding box for the arc to be drawn
		cosStart = cosDeg_helper(startAngle);
		sinStart = sinDeg_helper(startAngle);
		cosEnd = cosDeg_helper(endAngle);
		sinEnd = sinDeg_helper(endAngle);

		r = radius;
		// Point 1: radius & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 2: radius & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		r = radius - thickness;
		// Point 3: radius-thickness & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 4: radius-thickness & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;
		// Corrections if arc crosses X or Y axis
		if ((startAngle < 90) && (endAngle > 90)) ymax = radius;
		if ((startAngle < 180) && (endAngle > 180)) xmin = -radius;
		if ((startAngle < 270) && (endAngle > 270)) ymin = -radius;

		// Slopes for the two sides of the arc
		sslope = (float)cosStart / (float)sinStart;
		eslope = (float)cosEnd / (float)sinEnd;
		if (endAngle == 360) eslope = -1000000;
		ir2 = (radius - thickness) * (radius - thickness);
		or2 = radius * radius;
		startTransaction();
		for (x = xmin; x <= xmax; x++) {
			y1StartFound 	= false;
			y2StartFound 	= false;
			y1EndFound 		= false;
			y2EndSearching 	= false;
			y1s = 0;
			y1e = 0;
			y2s = 0;
			for (y = ymin; y <= ymax; y++) {
				x2 = x * x;
				y2 = y * y;
				if (
					(x2 + y2 < or2 && x2 + y2 >= ir2) && (
					(y > 0 && startAngle < 180 && x <= y * sslope) ||
					(y < 0 && startAngle > 180 && x >= y * sslope) ||
					(y < 0 && startAngle <= 180) ||
					(y == 0 && startAngle <= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)
					) && (
					(y > 0 && endAngle < 180 && x >= y * eslope) ||
					(y < 0 && endAngle > 180 && x <= y * eslope) ||
					(y > 0 && endAngle >= 180) ||
					(y == 0 && endAngle >= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)))
				{
					if (!y1StartFound) {	//start of the higher line found
						y1StartFound = true;
						y1s = y;
					} else if (y1EndFound && !y2StartFound) {//start of the lower line found
						y2StartFound = true;
						y2s = y;
						y += y1e - y1s - 1;	// calculate the most probable end of the lower line (in most cases the length of lower line is equal to length of upper line), in the next loop we will validate if the end of line is really there
						if (y > ymax - 1) {// the most probable end of line 2 is beyond ymax so line 2 must be shorter, thus continue with pixel by pixel search
							y = y2s;	// reset y and continue with pixel by pixel search
							y2EndSearching = true;
						}
					} else if (y2StartFound && !y2EndSearching) {
						// we validated that the probable end of the lower line has a pixel, continue with pixel by pixel search, in most cases next loop with confirm the end of lower line as it will not find a valid pixel
						y2EndSearching = true;
					}
				} else {
					if (y1StartFound && !y1EndFound) {//higher line end found
						y1EndFound = true;
						y1e = y - 1;
						drawFastVLine_cont(cx + x, cy + y1s, y - y1s, color);
						if (y < 0) {
							y = abs(y); // skip the empty middle
						} else {
							break;
						}
					} else if (y2StartFound) {
						if (y2EndSearching) {
							// we found the end of the lower line after pixel by pixel search
							drawFastVLine_cont(cx + x, cy + y2s, y - y2s, color);
							y2EndSearching = false;
							break;
						} else {
							// the expected end of the lower line is not there so the lower line must be shorter
							y = y2s;	// put the y back to the lower line start and go pixel by pixel to find the end
							y2EndSearching = true;
						}
					}
				}
			}
			if (y1StartFound && !y1EndFound){
				y1e = ymax;
				drawFastVLine_cont(cx + x, cy + y1s, y1e - y1s + 1, color);
			} else if (y2StartFound && y2EndSearching)	{// we found start of lower line but we are still searching for the end
														// which we haven't found in the loop so the last pixel in a column must be the end
				drawFastVLine_cont(cx + x, cy + y2s, ymax - y2s + 1, color);
			}
			#if defined(ESP8266)
				yield();
			#endif
		}
		closeTransaction();
	}
}

void SSD_13XX::setArcParams(float arcAngleMax, int arcAngleOffset)
{
	_arcAngleMax = arcAngleMax;
	_arcAngleOffset = arcAngleOffset;
}



/**************************************************************************/
/*!
		sin e cos helpers
		[private]
		from my RA8875 library
*/
/**************************************************************************/
float SSD_13XX::cosDeg_helper(float angle)
{
	float rads = angle / (float)360 * 2 * PI;
	return cos(rads);
}


float SSD_13XX::sinDeg_helper(float angle)
{
	float rads = angle / (float)360 * 2 * PI;
	return sin(rads);
}


//fast
void SSD_13XX::drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color)
{
	if (cx + radiusW >= _width) return;
	if (cy + radiusH >= _height) return;
	int16_t x,y;
	int16_t twoASquare,twoBSquare;
	int32_t stoppingX,stoppingY;
    int32_t xchange,ychange,ellipseError;
    twoASquare = 2 * (radiusW * radiusW);
    twoBSquare = 2 * (radiusH * radiusH);
    x = radiusW;
    y = 0;
    xchange = (radiusH * radiusH) * (1 - (2 * radiusW));
    ychange = (radiusW * radiusW);
    ellipseError = 0;
    stoppingX = (twoBSquare * radiusW);
    stoppingY = 0;
	startTransaction();
    while (stoppingX >= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		y++;
		stoppingY += twoASquare;
		ellipseError += ychange;
		ychange += twoASquare;
		if ((2 * ellipseError) + xchange > 0) {
			x--;
			stoppingX -= twoBSquare;
			ellipseError += xchange;
			xchange += twoBSquare;
		}
		#if defined(ESP8266)
			yield();
		#endif
    }
    x = 0;
    y = radiusH;
    xchange = (radiusH * radiusH);
    ychange = (radiusW * radiusW) * (1 - (2 * radiusH));
    ellipseError = 0;
    stoppingX = 0;
    stoppingY = (twoASquare * radiusH);
    while (stoppingX <= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		x++;
		stoppingX += twoBSquare;
		ellipseError += xchange;
		xchange += twoBSquare;
		if (((2 * ellipseError) + ychange) > 0) {
			y--;
			stoppingY -= twoASquare;
			ellipseError += ychange;
			ychange += twoASquare;
		}
		#if defined(ESP8266)
			yield();
		#endif
    }
	closeTransaction();
}

//fast
void SSD_13XX::plot4points_cont(int16_t cx, int16_t cy, int16_t x, int16_t y, uint16_t color)
{
	drawPixel_cont(cx + x, cy + y, color);
	if (x != 0) drawPixel_cont(cx - x, cy + y, color);
	if (y != 0) drawPixel_cont(cx + x, cy - y, color);
	if (x != 0 && y != 0) drawPixel_cont(cx - x, cy - y, color);
}

//fast
void SSD_13XX::drawCircle_cont_helper(int16_t x,int16_t y,int16_t radius,uint8_t cornername,uint16_t color)
{
	int16_t f			= 1 - radius;
	int16_t ddF_x		= 1;
	int16_t ddF_y 		= -2 * radius;
	int16_t deltax		= 0;
	int16_t deltay		= radius;
	if (cornername > 253){
		drawPixel_cont(x  		, y+radius	, color);
		drawPixel_cont(x  		, y-radius	, color);
		drawPixel_cont(x+radius	, y  		, color);
		drawPixel_cont(x-radius	, y  		, color);
	}
	while (deltax < deltay) {
		if (f >= 0) {
			deltay--;
			ddF_y += 2;
			f     += ddF_y;
		}
		deltax++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername > 253){
			drawPixel_cont(x + deltax, y + deltay, color);
			drawPixel_cont(x - deltax, y + deltay, color);
			drawPixel_cont(x + deltax, y - deltay, color);
			drawPixel_cont(x - deltax, y - deltay, color);
			drawPixel_cont(x + deltay, y + deltax, color);
			drawPixel_cont(x - deltay, y + deltax, color);
			drawPixel_cont(x + deltay, y - deltax, color);
			drawPixel_cont(x - deltay, y - deltax, color);
		} else {
			if (cornername & 0x4) {
				drawPixel_cont(x + deltax, y + deltay, color);
				drawPixel_cont(x + deltay, y + deltax, color);
			}
			if (cornername & 0x2) {
				drawPixel_cont(x + deltax, y - deltay, color);
				drawPixel_cont(x + deltay, y - deltax, color);
			}
			if (cornername & 0x8) {
				drawPixel_cont(x - deltay, y + deltax, color);
				drawPixel_cont(x - deltax, y + deltay, color);
			}
			if (cornername & 0x1) {
				drawPixel_cont(x - deltay, y - deltax, color);
				drawPixel_cont(x - deltax, y - deltay, color);
			}
		}
		#if defined(ESP8266)
			yield();
		#endif
	}
}



//fast
void SSD_13XX::fillCircle_cont_helper(int16_t x, int16_t y, int16_t radius, uint8_t cornername, int16_t delta, uint16_t color)
{
	int16_t f     		= 1 - radius;
	int16_t ddF_x 		= 1;
	int16_t ddF_y 		= -2 * radius;
	int16_t deltax		= 0;
	int16_t deltay		= radius;

	while (deltax < deltay) {
		if (f >= 0) {
			deltay--;
			ddF_y += 2;
			f     += ddF_y;
		}
		deltax++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine_cont(x+deltax, y-deltay, 2*deltay+1+delta, color);
			drawFastVLine_cont(x+deltay, y-deltax, 2*deltax+1+delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine_cont(x-deltax, y-deltay, 2*deltay+1+delta, color);
			drawFastVLine_cont(x-deltay, y-deltax, 2*deltax+1+delta, color);
		}
		#if defined(ESP8266)
			yield();
		#endif
	}
}

//fast
void SSD_13XX::drawCircle(int16_t x, int16_t y, int16_t radius, uint16_t color)
{
	if (x+(radius) >= _width || y+(radius) >= _height) return;//radius*2

	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t xrel = 0;
	int16_t yrel = radius;
	startTransaction();
	drawPixel_cont(x  , y+radius, color);
	drawPixel_cont(x  , y-radius, color);
	drawPixel_cont(x+radius, y  , color);
	drawPixel_cont(x-radius, y  , color);

	while (xrel<yrel) {
		if (f >= 0) {
			yrel--;
			ddF_y += 2;
			f += ddF_y;
		}
		xrel++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel_cont(x + xrel, y + yrel, color);
		drawPixel_cont(x - xrel, y + yrel, color);
		drawPixel_cont(x + xrel, y - yrel, color);
		drawPixel_cont(x - xrel, y - yrel, color);
		drawPixel_cont(x + yrel, y + xrel, color);
		drawPixel_cont(x - yrel, y + xrel, color);
		drawPixel_cont(x + yrel, y - xrel, color);
		drawPixel_cont(x - yrel, y - xrel, color);
	}
	closeTransaction();
}

void SSD_13XX::fillCircle(int16_t x, int16_t y, int16_t radius,uint16_t color)
{
	if (x+(radius) >= _width || y+(radius) >= _height) return;//radius*2
	startTransaction();//open SPI comm
	drawFastVLine_cont(x, y-radius, (2*radius)+1, color);
	fillCircle_cont_helper(x, y, radius, 3, 0, color);
	closeTransaction();
}


//fast
void SSD_13XX::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t radius, uint16_t color)
{
	if (x+w >= _width || y+h > _height) return;
	//if (x+w-radius-1 < 0 || y+h-radius-1 < 0) return;
	startTransaction();
	drawFastHLine_cont(x+radius  , y    	 , w-2*radius, color); // Top
	drawFastHLine_cont(x+radius  , y+h-1	 , w-2*radius, color); // Bottom
	drawFastVLine_cont(x    	 , y+radius  , h-2*radius, color); // Left
	drawFastVLine_cont(x+w-1	 , y+radius  , h-2*radius, color); // Right
  // draw four corners
	drawCircle_cont_helper(x+radius    , y+radius    , radius, 1, color);
	drawCircle_cont_helper(x+w-radius-1, y+radius    , radius, 2, color);
	drawCircle_cont_helper(x+w-radius-1, y+h-radius-1, radius, 4, color);
	drawCircle_cont_helper(x+radius    , y+h-radius-1, radius, 8, color);
	closeTransaction();
}


//fast
void SSD_13XX::fillRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t radius, uint16_t color)
{
	if (x+w >= _width || y+h > _height) return;
	if (radius < 2) {
		fillRect(x,y,w,h,color);
		return;
	}
	startTransaction();
	drawRect_cont(x+radius, y, w-2*radius, h, color, color,1);
	//fillRect_cont(x+radius, y, w-2*radius, h, color, color);
	fillCircle_cont_helper(x+w-radius-1, y+radius, radius, 1, h-2*radius-1, color);
	fillCircle_cont_helper(x+radius    , y+radius, radius, 2, h-2*radius-1, color);
	closeTransaction();
}


void SSD_13XX::drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color)
{
	startTransaction();//open SPI comm
	drawLine_cont(x0, y0, x1, y1, color/*,20*/);//low 1
	drawLine_cont(x1, y1, x2, y2, color/*,20*/);//high 1
	drawLine_cont(x2, y2, x3, y3, color/*,20*/);//high 2
	drawLine_cont(x3, y3, x0, y0, color/*,20*/);//low 2
	closeTransaction();
}


void SSD_13XX::fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color,bool triangled)
{
	startTransaction();//open SPI comm
    fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);
	if (triangled) fillTriangle_cont(x2, y2, x3, y3, x0, y0, color);
    fillTriangle_cont(x1,y1,x2,y2,x3,y3,color);
	closeTransaction();
}

void SSD_13XX::drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rot, uint16_t color)
{
	sides = (sides > 2 ? sides : 3);
	const float dtr = (PI/180.0) + PI;
	float rads = 360.0 / sides;//points spacd equally
	uint8_t i;
	startTransaction();
	for (i = 0; i < sides; i++) {
		drawLine_cont(
			x + (sin((i*rads + rot) * dtr) * diameter),
			y + (cos((i*rads + rot) * dtr) * diameter),
			x + (sin(((i+1)*rads + rot) * dtr) * diameter),
			y + (cos(((i+1)*rads + rot) * dtr) * diameter),
			color//,20
			);
	}
	closeTransaction();
}

void SSD_13XX::drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	if (boundaryCheck(x,y)) return;
	//if (((x + w) - 1) >= _width)  w = _width  - x;
	//if (((y + h) - 1) >= _height) h = _height - y;
	w = sizeCheck(x,w,_width);
	h = sizeCheck(y,h,_height);

	int16_t n, m;

	if (w < x) {n = w; w = x; x = n;}
	if (h < y) {n = h; h = y; y = n;}
	startTransaction();
	for (m = y; m <= h; m += 2) {
		for (n = x; n <= w; n += 2) {
			drawPixel_cont(n, m, color);
		}
	}
	closeTransaction();
}

void SSD_13XX::drawTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
	if (x0 > _width || x1 > _width || x2 > _width) return;
	if (y0 > _height || y1 > _height || y2 > _height) return;
	startTransaction();
	drawLine_cont(x0, y0, x1, y1, color/*,800*/);
	drawLine_cont(x1, y1, x2, y2, color/*,800*/);
	drawLine_cont(x2, y2, x0, y0, color/*,800*/);
	closeTransaction();
}

//85% fast
void SSD_13XX::fillTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
	startTransaction();
	fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);//
	closeTransaction();
}

void SSD_13XX::fillTriangle_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;

	if (y0 > y1) {swapVals(y0, y1); swapVals(x0, x1);}
	if (y1 > y2) {swapVals(y2, y1); swapVals(x2, x1);}
	if (y0 > y1) {swapVals(y0, y1); swapVals(x0, x1);}

	if (y0 == y2) {
		a = b = x0;
		if (x1 < a){
			a = x1;
		} else if (x1 > b) {
			b = x1;
		}
		if (x2 < a){
			a = x2;
		} else if (x2 > b) {
			b = x2;
		}
		drawFastHLine_cont(a, y0, b-a+1, color);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa   = 0,
		sb   = 0;

	if (y1 == y2) {
		last = y1;
	} else {
		last = y1-1;
	}

	for (y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b) swapVals(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y <= y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b) swapVals(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}
}


/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void SSD_13XX::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t length, uint16_t color,int offset)
{
	if (length < 2) {//NEW
		drawPixel(x,y,color);
	} else {
		drawLine(
		x,
		y,
		x + (length * cosDeg_helper(angle + offset)),//_angle_offset
		y + (length * sinDeg_helper(angle + offset)),
		color);
	}
}

/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  start: where line start
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void SSD_13XX::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, uint16_t color,int offset)
{
	if (start - length < 2) {//NEW
		drawPixel(x,y,color);
	} else {
		drawLine(
		x + start * cosDeg_helper(angle + offset),//_angle_offset
		y + start * sinDeg_helper(angle + offset),
		x + (start + length) * cosDeg_helper(angle + offset),
		y + (start + length) * sinDeg_helper(angle + offset),
		color);
	}
}

/**************************************************************************/
/*!
      ringMeter
	  (adapted from Alan Senior (thanks man!))
	  from my RA8875 library
	  it create a ring meter with a lot of personalizations,
	  it return the width of the gauge so you can use this value
	  for positioning other gauges near the one just created easily
	  Parameters:
	  val:  your value
	  minV: the minimum value possible
	  maxV: the max value possible
	  x:    the position on x axis
	  y:    the position on y axis
	  r:    the radius of the gauge (minimum 20)
	  units: a text that shows the units, if "none" all text will be avoided
	  scheme:0...7 or 16 bit color (not BLACK or WHITE)
	  0:red
	  1:green
	  2:blue
	  3:blue->red
	  4:green->red
	  5:red->green
	  6:red->green->blue
	  7:cyan->green->red
	  8:black->white linear interpolation
	  9:violet->yellow linear interpolation
	  or
      RGB565 color (not BLACK or WHITE)
	  backSegColor: the color of the segments not active (default BLACK)
	  angle:		90 -> 180 (the shape of the meter, 90:halfway, 180:full round, 150:default)
	  inc: 			5...20 (5:solid, 20:sparse divisions, default:10)
*/
/**************************************************************************/
void SSD_13XX::ringMeter(int val, int minV, int maxV, uint8_t x, uint8_t y, uint8_t r, uint16_t colorScheme,uint16_t backSegColor,int angle,uint8_t inc)
{
	if (inc < 5) inc = 5;
	if (inc > 20) inc = 20;
	if (r < 50) r = 20;
	if (angle < 90) angle = 90;
	if (angle > 180) angle = 180;
	int i;
	int curAngle = map(val, minV, maxV, -angle, angle);
	uint16_t colour;
	x += r;
	y += r;   // Calculate coords of centre of ring
	uint8_t w = r / 4;    // Width of outer ring is 1/4 of radius
	const uint8_t seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
	// Draw colour blocks every inc degrees
	for (i = -angle; i < angle; i += inc) {
		colour = BLACK;
		switch (colorScheme) {
			case 0:
				colour = RED;
				break; // Fixed colour
			case 1:
				colour = GREEN;
				break; // Fixed colour
			case 2:
				colour = BLUE;
				break; // Fixed colour
			case 3:
				colour = gradient(map(i, -angle, angle, 0, 127));
				break; // Full spectrum blue to red
			case 4:
				colour = gradient(map(i, -angle, angle, 63, 127));
				break; // Green to red (high temperature etc)
			case 5:
				colour = gradient(map(i, -angle, angle, 127, 63));
				break; // Red to green (low battery etc)
			case 6:
				colour = gradient(map(i, -angle, angle, 127, 0));
				break; // Red to blue (air cond reverse)
			case 7:
				colour = gradient(map(i, -angle, angle, 35, 127));
				break; // cyan to red
			case 8:
				colour = colorInterpolation(0,0,0,255,255,255,map(i,-angle,angle,0,w),w);
				break; // black to white
			case 9:
				colour = colorInterpolation(0x80,0,0xC0,0xFF,0xFF,0,map(i,-angle,angle,0,w),w);
				break; // violet to yellow
			default:
				if (colorScheme > 9){
					colour = colorScheme;
				} else {
					colour = BLUE;
				}
				break; // Fixed colour
		}
		// Calculate pair of coordinates for segment start
		float xStart = cos((i - 90) * 0.0174532925);
		float yStart = sin((i - 90) * 0.0174532925);
		uint8_t x0 = xStart * (r - w) + x;
		uint8_t y0 = yStart * (r - w) + y;
		uint8_t x1 = xStart * r + x;
		uint8_t y1 = yStart * r + y;

		// Calculate pair of coordinates for segment end
		float xEnd = cos((i + seg - 90) * 0.0174532925);
		float yEnd = sin((i + seg - 90) * 0.0174532925);
		uint8_t x2 = xEnd * (r - w) + x;
		uint8_t y2 = yEnd * (r - w) + y;
		uint8_t x3 = xEnd * r + x;
		uint8_t y3 = yEnd * r + y;

		if (i < curAngle) { // Fill in coloured segments with 2 triangles
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, colour, false);
		} else {// Fill in blank segments
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, backSegColor, false);
		}
	}

}

//++++++++++++++OK
void SSD_13XX::startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	startTransaction();
	if (_portrait) {
		swapVals(x0,y0);
		swapVals(x1,y1);
	}
	setAddrWindow_cont(x0,y0,x1,y1,false);
}


//++++++++++++++OK
void SSD_13XX::pushData(uint16_t color)
{
	writedata16_cont(color);
}

//++++++++++++++OK
void SSD_13XX::endPushData()
{
	closeTransaction();
}

//fast
void SSD_13XX::pushColor(uint16_t color)
{
	startTransaction();
	writedata16_last(color);
	endTransaction();
}



void SSD_13XX::drawIcon(int16_t x, int16_t y,const tIcon *icon,uint8_t scale,uint16_t f,uint16_t b,bool inverse)
{
	#if defined(_SSD_1351_REG_H_)
		if (_portrait) swapVals(x,y);
	#endif
	#if defined(_FORCE_PROGMEM__)
		const _smCharType * iconData 	= PROGMEM_read(&icon->data);
		uint8_t		iWidth				= pgm_read_byte(&(icon->image_width));	//AVR ok
		uint8_t		iHeight				= pgm_read_byte(&(icon->image_height)); //AVR ok
		uint16_t	datalen				= pgm_read_word(&(icon->image_datalen));//AVR ok
		//boolean	dataComp			= PROGMEM_read(&icon->image_comp);//not yet
	#else
		const _smCharType * iconData	= icon->data;//icon data
		uint8_t		iWidth				= icon->image_width;
		uint8_t		iHeight				= icon->image_height;
		uint16_t	datalen				= icon->image_datalen;
		//uint8_t		dataComp		= icon->image_comp;//not yet
	#endif

	if (iWidth < 1 || iHeight < 1) return;//cannot be
	if (scale < 1) scale = 1;
	if ((x + iWidth) * scale >= _width || (y + iHeight) * scale >= _height) return;//cannot be

	startTransaction();
	//LGPO Rendering (uncomp)
	if (!_portrait){
		setAddrWindow_cont(x,y,iWidth+x,iHeight+y,false);
	_glyphRender_unc(
					iconData,
					x,
					y,
					iWidth,
					iHeight,
					scale,
					scale,
					datalen,
					0,
					f,
					b,
					inverse
	);
	} else {
		//setAddrWindow_cont(y,x,iWidth+y,iHeight+x,false);
		setAddrWindow_cont(y,x,iWidth+y,iHeight+x,false);
	_glyphRender_unc(
					iconData,
					y,
					x,
					iWidth,
					iHeight,
					scale,
					scale,
					datalen,
					0,
					f,
					b,
					inverse
	);
	}
	closeTransaction();
}


//OK with SetRemap 0...3
void SSD_13XX::drawImage(int16_t x, int16_t y,const tPicture *img,const enum SSD_13XX_iconMods m,uint16_t b)
{
	uint16_t px = 0;
	uint16_t color;
	uint8_t currentY = 0;
	uint16_t currentX = 0;
	bool skip = false;
	#if defined(_FORCE_PROGMEM__)
		const uint16_t * imageData  = PROGMEM_read(&img->data);
		uint8_t		iWidth			= pgm_read_byte(&img->image_width);
		uint8_t		iHeight			= pgm_read_byte(&img->image_height);
		uint16_t	datalen			= pgm_read_word(&img->image_datalen);
		//uint8_t		dataDepth	= pgm_read_byte(&img->image_depth);//not yet
		//boolean		dataComp	= pgm_read_byte(&(img->image_comp);//not yet
	#else
		const uint16_t * imageData	= img->data;//image data
		uint8_t		iWidth			= img->image_width;
		uint8_t		iHeight			= img->image_height;
		uint16_t	datalen			= img->image_datalen;
		//uint8_t		dataDepth	= img->image_depth;//not yet
		//uint8_t		dataComp	= img->image_comp;//not yet
	#endif
	iWidth -= 1;
	iHeight -= 1;
	if (iWidth < 1 || iHeight < 1) return;//cannot be
	if (_portrait) {
		swapVals(x,y);
		swapVals(iWidth,iHeight);
	}

	if (x + iWidth >= _width || y + iHeight >= _height) return;//cannot be

	startTransaction();
	setAddrWindow_cont(x,y,iWidth+x,iHeight+y,false);

	do {
		//get the pixel
		#if defined(_FORCE_PROGMEM__)
			color = pgm_read_word(&imageData[px]);
		#else
			color = imageData[px];
		#endif
		//simple color operations
		if (m == TRANSPARENT){
			if (color <= b) skip = true;
		} else if (m == REPLACE){
			if (color <= b) color = _defaultBgColor;
		} else if (m == BOTH){
			if (color <= b) {
				color = _defaultBgColor;
			} else {
				color = _defaultFgColor;
			}
		}

		if (!skip) {
			writedata16_cont(color);
		} else {
			setAddrWindow_cont(x+currentX,y+currentY,(iWidth+x),(iHeight+y),false);//constrain window
			skip = false;
		}

		if (currentX == iWidth){
			currentX = 0;
			currentY++;
		} else {
			currentX++;
		}

		px++;
	} while (--datalen > 0);

	closeTransaction();
}


void SSD_13XX::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color)
{
	int16_t i, j;
	int16_t byteWidth = (w + 7) / 8;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) drawPixel(x + i, y + j, color);
		}
		#if defined(ESP8266)
			yield();
		#endif
	}
}

void SSD_13XX::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg)
{
	int16_t i, j;
	int16_t byteWidth = (w + 7) / 8;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
				drawPixel(x + i, y + j, color);
			} else {
				drawPixel(x + i, y + j, bg);
		}
    }
	#if defined(ESP8266)
		yield();
	#endif
  }
}

/*********************************************************
********************  Text Functions *********************
**********************************************************/
void SSD_13XX::setCursor(int16_t x, int16_t y,enum SSD_13XX_centerMode c)
{
	if (c == NORM){// No centering
		if (x == CENTER || y == CENTER) setCursor(x,y,SCREEN);
		if (boundaryCheck(x,y)) return;//cannot be
	} else if (c == SCREEN){// Absolute x,y or both center screen mode
		if (x == CENTER && y == CENTER) {//center x,y
			x = y = 0;//better reset since value is calculated in textWrite
			_centerText = 3;
		} else if (x == CENTER && y != CENTER) {//center on x
			if (y > _height) return;//cannot be
			x = 0;//better reset since value is calculated in textWrite
			_centerText = 1;
		} else if (y == CENTER && x != CENTER) {//center on y
			if (x > _width) return;//cannot be
			y = 0;//better reset since value is calculated in textWrite
			_centerText = 2;
		} else {
			setCursor(x,y,NORM);
		}
	} else {// Relative to x,y or both center mode
		if (x == CENTER || y == CENTER) setCursor(x,y,SCREEN);
		if (c == REL_X){//relative to X
			_centerText = 4;
		} else if (c == REL_Y){//relative to Y
			_centerText = 5;
		} else if (c == REL_XY){//relative XY
			_centerText = 6;
		}
	}
	if (_portrait) swapVals(x,y);
	_cursorX = x;
	_cursorY = y;
	setArea(0x0000,0x0000,x,y);
}

void SSD_13XX::getCursor(int16_t &x, int16_t &y)
{
	x = _cursorX;
	y = _cursorY;
}


void SSD_13XX::setTextScale(uint8_t s)
{
	_textScaleX = _textScaleY = (s > 0) ? s : 1;
}


void SSD_13XX::setTextScale(uint8_t sx,uint8_t sy)
{
	_textScaleX = (sx > 0) ? sx : 1;
	_textScaleY = (sy > 0) ? sy : 1;
}

void SSD_13XX::setTextColor(uint16_t color)
{
	_textForeground = _textBackground = color;
}

void SSD_13XX::setTextColor(uint16_t frgrnd, uint16_t bckgnd)
{
	_textForeground = frgrnd;
	_textBackground = bckgnd;
}


void SSD_13XX::setTextWrap(boolean w)
{
	_textWrap = w;
}

void SSD_13XX::setCharSpacing(uint8_t space)
{
	_charSpacing = space;
}

void SSD_13XX::setFontInterline(uint8_t distance)
{
	_fontInterline = distance;
}

void SSD_13XX::setInternalFont(void)
{
	#if defined(_SSD_DEF_FONT_PATH)
		setFont(&_SSD_DEF_FONT_NAME);
	#else
		setFont(&nullfont);
	#endif
}


int SSD_13XX::_getCharCode(uint8_t ch)
{
	int i;
	//remap ?
	if (_fontRemapOffset == 1 && (ch > 96 && ch < 123)){
		ch -= 32;
	} else if (_fontRemapOffset == 2 && ((ch > 64 && ch < 91))){
		ch += 32;
	}
	//search for char code
	for (i = 0;i < _currentFont->length;i++){
		if (_currentFont->chars[i].char_code == ch) return i;
	}
	return -1;
}


/*
	Return the lenght of a string in pixel with precision
*/
int SSD_13XX::_STRlen_helper(const char* buffer,int len)
{
	int charIndex = -1;
	int i;
	//if (len < 1) len = strlen(buffer);		//try to get data from string
	//if (len < 1) return 0;					//better stop here
	if (_currentFont->font_widthType != 0){		// fixed width font
		return ((len * _spaceCharWidth));
	} else {								// variable width, need to loop trough entire string!
		int totW = 0;
		for (i = 0;i < len;i++){			//loop trough buffer
			if (buffer[i] == 32){			//a space
				totW += _spaceCharWidth;
			} else if (buffer[i] != 13 && buffer[i] != 10 && buffer[i] != 32){//avoid special char
				charIndex = _getCharCode(buffer[i]);
				if (charIndex > -1) {		//found!
					#if defined(_FORCE_PROGMEM__)
						totW += (pgm_read_byte(&(_currentFont->chars[charIndex].image->image_width)));
					#else
						totW += (_currentFont->chars[charIndex].image->image_width);
					#endif
					totW += _charSpacing;
				}
			}//inside permitted chars
		}//buffer loop
		return totW;
	}//end variable w font
}


void SSD_13XX::setFont(const tFont *font)
{
	_currentFont = font;
	_fontRemapOffset =  _currentFont->remap_code;
	//get all needed infos
	// We just get the space width now...
	int temp = _getCharCode(0x20);
	if (temp > -1){
		#if defined(_FORCE_PROGMEM__)
			_spaceCharWidth = pgm_read_byte(&(_currentFont->chars[temp].image->image_width));
		#else
			_spaceCharWidth = (_currentFont->chars[temp].image->image_width);
		#endif
	} else {
		//font malformed, doesn't have needed space parameter will return to system font
		#if defined(_SSD_DEF_FONT_PATH)
			setFont(&_SSD_DEF_FONT_NAME);
		#else
			setFont(&nullfont);
		#endif
		return;
	}
}

/*
Handle strings
*/
void SSD_13XX::_textWrite(const char* buffer, uint16_t len)
{
	uint16_t i;
	if (len < 1) len = strlen(buffer);//try get the info from the buffer
	if (len < 1) return;//better stop here, the string it's really empty!
	// Center text flag enabled
	if (_centerText > 0){
		uint8_t stringWide = (_STRlen_helper(buffer,len) * _textScaleX) / 2;
		uint8_t strMidHeight = (((_currentFont->font_height - _currentFont->font_descent) * _textScaleY) / 2);
		if (_centerText < 4) {
			//absolute
			if (_portrait){
				if (_centerText > 1)  _cursorX = (_height / 2) - strMidHeight;
				if (_centerText != 2) _cursorY = (_width / 2) - stringWide;
			} else {
				if (_centerText > 1)  _cursorY = (_height / 2) - strMidHeight;
				if (_centerText != 2) _cursorX = (_width / 2) - stringWide;
			}
		} else {
			//relative
			if (_portrait){
				if (_centerText > 4)  _cursorX = _cursorX - strMidHeight;
				if (_centerText != 5) _cursorY = _cursorY - stringWide;
			} else {
				if (_centerText > 4)  _cursorY = _cursorY - strMidHeight;
				if (_centerText != 5) _cursorX = _cursorX - stringWide;
			}
		}
		if (_cursorX < 0)_cursorX = 0;
		if (_cursorY < 0)_cursorY = 0;
		_centerText = 0;//reset
	}//end center flag
	//Loop trough every char and write them one by one until end (or a break!)
	startTransaction();
	for (i=0;i<len;i++){
		if (_renderSingleChar(buffer[i])) {
			//aha! in that case I have to break out!
			break;
		}
		/*
	#if defined(ESP8266)
		yield();
	#endif
	*/
	}//end loop
	closeTransaction();
}

/*
Preprocessor for single chars
This function detect carriage/new line and space and perform separately.
When a char it's detected it pass the info to the drawChar function.
It return 0 most of the cases but can return 1 to inform the caller function to break
the string write loop.
*/
bool SSD_13XX::_renderSingleChar(const char c)
{
	uint8_t borderRight = 0;
	uint8_t borderBottom = 0;
	if (c == 13){//------------------------------- CARRIAGE (detected) -----------------------
		return 0;//ignore, always OK
	} else if (c == 10){//------------------------- NEW LINE (detected) -----------------------
		borderBottom = (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
		if (!_portrait){
			//borderBottom = (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
			if (_cursorY + borderBottom  > _height) return 1;//too high!
			_cursorX = 0;
			_cursorY += borderBottom;
		} else {//portrait
			//borderBottom = (_currentFont->font_height * _textScaleX) + (_fontInterline * _textScaleX);
			if (_cursorX + borderBottom  > _width) return 1;//too high!
			_cursorX += borderBottom;
			_cursorY = 0;
		}
		return 0;
	} else if (c == 32){//--------------------------- SPACE (detected) -----------------------
		if (!_portrait){
			borderRight = (_spaceCharWidth * _textScaleX) + (_charSpacing * _textScaleX);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorX + borderRight >= _width) borderRight = _width - _cursorX;
				//fillRect_cont(
				drawRect_cont(
					_cursorX,
					_cursorY,
					borderRight + (_charSpacing * _textScaleX),
					(_currentFont->font_height * _textScaleY),
					_textBackground,
					_textBackground,
					1
				);
			}
			_cursorX += borderRight;
			return 0;
		} else {//portrait
			borderRight = (_spaceCharWidth * _textScaleY) + (_charSpacing * _textScaleY);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorY + borderRight >= _height) borderRight = _height - _cursorY;
				//fillRect_cont(
				drawRect_cont(
					_cursorY,
					_cursorX,
					borderRight,
					(_currentFont->font_height * _textScaleX),
					_textBackground,
					_textBackground,
					1
				);
			}
			_cursorY += borderRight;
			return 0;
		}
	} else {//-------------------------------------- CHAR  (detected) -------------------------
		int charIndex = _getCharCode(c);//get char code
		if (charIndex > -1){//check if it's valid
			int charW = 0;
			//I need to know the width...
			#if defined(_FORCE_PROGMEM__)
				charW = pgm_read_byte(&(_currentFont->chars[charIndex].image->image_width));
			#else
				charW = _currentFont->chars[charIndex].image->image_width;
			#endif
			//---------------------------------- WRAP is ON? --------------------------------
			if (_textWrap){//wrap, goes in the new line
				if (!_portrait && (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX)) >= _width){
					_cursorX = 0;
					_cursorY += (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
				} else if (_portrait && (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY)) >= _width){
					_cursorX += (_currentFont->font_height * _textScaleX) + (_fontInterline * _textScaleX);
					_cursorY = 0;
				}
			} else {//not wrap, will get rid of the data
				if (_portrait){
					if (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY) >= _width) return 1;
				} else {
					if (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX) >= _width) return 1;
				}
			}
			//-------------------------Actual single char drawing here -----------------------------------
			//updated in 1.0p7
			#if defined(_FORCE_PROGMEM__)
				const _smCharType * charGlyp = PROGMEM_read(&_currentFont->chars[charIndex].image->data);//char data
				int		totalBytes = pgm_read_word(&(_currentFont->chars[charIndex].image->image_datalen));
			#else
				const _smCharType * charGlyp = _currentFont->chars[charIndex].image->data;
				int		totalBytes = _currentFont->chars[charIndex].image->image_datalen;
			#endif
			if (!_portrait){
				if (_cursorY + (_currentFont->font_height * _textScaleY) > _height) return 1;//too high!
				_glyphRender_unc(
								charGlyp,
								_cursorX,
								_cursorY,
								charW,
								_currentFont->font_height,
								_textScaleX,
								_textScaleY,
								totalBytes,
								_charSpacing,
								_textForeground,
								_textBackground,
								false
				);
				_cursorX += (charW * _textScaleX) + (_charSpacing * _textScaleX);//add charW to total
			} else {
				if (_cursorX + (_currentFont->font_height * _textScaleX) > _width) return 1;//too high!
				_glyphRender_unc(
								charGlyp,
								_cursorY,
								_cursorX,
								charW,
								_currentFont->font_height,
								_textScaleX,
								_textScaleY,
								totalBytes,
								_charSpacing,
								_textForeground,
								_textBackground,
								false
				);
				_cursorY += (charW * _textScaleX) + (_charSpacing * _textScaleY);//add charW to total
			}
			return 0;
		}//end valid
		return 0;
	}//end char
}

/*
 - LGPO - rendering engine, part 1 (GNU v3)
This is the draw char function (version for uncompressed font)
It detects blank and filled lines and render separately, this is the first
accelleration step of the unique (and currently under commercial licence) sumotoy render engine,
it's a variation of LPGO font render accelleration used in RA8875 (under GNU v3).
The lines are not blank or filled are passed to the grouping function that is the second part of the accelleration.
*/
void SSD_13XX::_glyphRender_unc(
									const 		_smCharType *pixelsArray,
									int16_t 	x,
									int16_t 	y,
									int 		glyphWidth,
									int 		glyphHeight,
									uint8_t 	scaleX,
									uint8_t	 	scaleY,
									uint16_t 	totalBytes,
									uint8_t 	cspacing,
									uint16_t 	foreColor,
									uint16_t 	backColor,
									bool 		inverse
									)
{
	//start by getting some glyph data...
	int i;
	uint8_t temp = 0;
	//some basic variable...
	uint8_t currentXposition = 0;//the current position of the writing cursor in the x axis, from 0 to glyphWidth
	uint8_t currentYposition = 0;//the current position of the writing cursor in the y axis, from 0 to _FNTheight
	uint8_t tempYpos = 0;
	uint16_t currentByte = 0;//the current byte in reading (from 0 to totalBytes)
	bool lineBuffer[glyphWidth+1];//the temporary line buffer
	int lineChecksum = 0;//part of the optimizer
	//Fill background if needed.
	//if (foreColor != backColor) fillRect_cont(x,y,((glyphWidth * scaleX) + (cspacing * scaleX)),(glyphHeight * scaleY),backColor,backColor);
	if (foreColor != backColor) drawRect_cont(x,y,((glyphWidth * scaleX) + (cspacing * scaleX)),(glyphHeight * scaleY),backColor,backColor,1);
	//the main loop that will read all bytes of the glyph
	while (currentByte < totalBytes){
		//delay(1);
		//read n byte
		#if defined(_FORCE_PROGMEM__)
			temp = pgm_read_byte(&(pixelsArray[currentByte]));
		#else
			temp = pixelsArray[currentByte];
		#endif
		if (inverse) temp = ~temp;//invert byte if needed
		//read current bits inside current byte
		for (i = 7; i >= 0; i--){
			lineBuffer[currentXposition] = bitRead(temp,i);//continue fill line buffer
			lineChecksum += lineBuffer[currentXposition++];
			//----------------------------------- exception
			if (currentXposition == glyphWidth){
				//line buffer has been filled!
				currentXposition = 0;//reset the line x position
				tempYpos = y + (currentYposition * scaleY);
				if (lineChecksum < 1){
					//do nothing
				} else if (lineChecksum == glyphWidth){
					//full line
					//drawRect_cont(
					fillRect_cont(
							x,
							tempYpos,
							(glyphWidth * scaleX),
							scaleY,
							foreColor,
							backColor//,//backColor
							//1
					);
				} else {
					//line render
					_charLineRender(
							lineBuffer,
							glyphWidth,
							x,
							y,
							scaleX,
							scaleY,
							currentYposition,
							foreColor
					);
				}
				currentYposition++;//next line
				lineChecksum = 0;//reset checksum
			}//end exception
		}//end reading single byte
		currentByte++;
	}
}

/*
 - LGPO - rendering engine, part 2 (GNU v3)
LPGO font render accelleration (GNU v3), part 2, pixel grouping.
the sumotoy proprietary line render engine, please do not steal
without author permission since there's currently some licence on it!
This function group pixels with same color and perform much less memory addressing
than any other similar function I ever seen.
Here has been used to avoid multiple memory addressing but can be inproved, the LPGO shines
where harware accelleration it's present but this chip has only direct memory access...
*/
void SSD_13XX::_charLineRender(
									bool 			lineBuffer[],
									int 			charW,
									int16_t 		x,
									int16_t 		y,
									uint8_t 		scaleX,
									uint8_t 		scaleY,
									int16_t 		currentYposition,
									uint16_t 		foreColor
									)
{

	int xlinePos = 0;
	int px;
	uint8_t endPix = 0;
	bool refPixel = false;
	while (xlinePos < charW){
		refPixel = lineBuffer[xlinePos];//xlinePos pix as reference value for next pixels
		//detect and render concurrent pixels
		for (px = xlinePos;px <= charW;px++){
			if (lineBuffer[px] == lineBuffer[xlinePos] && px < charW){
				//grouping pixels with same val
				endPix++;
			} else {
				if (refPixel) {
						//fillRect_cont(
						drawRect_cont(
						x,
						y + (currentYposition * scaleY),
						(endPix * scaleX),
						scaleY,
						foreColor,
						foreColor,
						1
					);
				}
				//reset and update some vals
				xlinePos += endPix;
				x += endPix * scaleX;
				endPix = 0;
				break;//exit cycle for...
			}
		}
	}//while
}


/*
 ----------------- PushColor stream --------------------------------
*/
#if defined(__AVR__)
	void SSD_13XX::_pushColors_cont(uint16_t data,uint32_t times){
		uint8_t i;
		enableDataStream();
		while(times--) {
			for (i=0;i<2;i++){
				while(!(SPSR & (1 << SPIF)));
				SPDR = (data >> (8 - (i*8)));
			}
		}
		while(!(SPSR & (1 << SPIF)));
	}
#elif defined(__SAM3X8E__)
	void SSD_13XX::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
		while(times--) { SPI.transfer16(data); }
	}
#elif defined(__MKL26Z64__)
	void SSD_13XX::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
		while(times--) {
			if (_useSPI1){
				SPI1.transfer16(data);
			} else {
				SPI.transfer16(data);
			}
		}
	}
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	void SSD_13XX::_pushColors_cont(uint16_t data,uint32_t times)
	{
		do {
			writedata16_cont(data);
		} while (--times > 0);
	}
#elif defined(ESP8266)
	void SSD_13XX::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
		while(times--) { spiwrite16(data); }
		//alternative faster (but currently not work at 80Mhz or more)
		//uint8_t pattern[2] = { (uint8_t)(data >> 8), (uint8_t)(data >> 0) };
		//SPI.writePattern(pattern, 2, times);
	}
#else
	void SSD_13XX::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
			while(times--) {
			SPI.transfer(data >> 8); SPI.transfer(data);
		}
	}
#endif
/* -------------------------------------------------------------------------------------------
++++++++++++++++++++++++++++++++ Size Optimizations ++++++++++++++++++++++++++++++++++++++++++
always inline routines dublicate the entire function inside chunks of code for speed during
compiling, this is acceptable for cpu with lot of code space (like Teensy 3.x, DUE) but
can increase dramatically the amount of code for UNO and similar.
For this reason some function are now normal for all CPU, this decrease a fraction of the
overall speed but decrease the amount of space occupied by code.
In addition, there's an user option to decrease a lot the space, enabled for small resources CPU.
I'm sorry for the complicated preprocessor #if #else and the amount of code inside library for
fix this but is the only 'fast way' I found to acieve this!
--------------------------------------------------------------------------------------------*/


#if defined(_SSD_SIZEOPTIMIZER)
/* ========================================================================
	-------------------- Common low level commands ------------------------
	Teensy 3.x uses different functions, This are for all the rest of MCU's
   ========================================================================*/
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__) && !defined(__MK64FX512__) && !defined(__MK66FX1M0__)
		void SSD_13XX::writecommand_cont(const uint8_t c)
		{
			enableCommandStream(); spiwrite(c);
		}

		void SSD_13XX::writecommand16_cont(const uint16_t c)
		{
			enableCommandStream(); spiwrite16(c);
		}

		void SSD_13XX::writedata8_cont(uint8_t c)
		{
			enableDataStream(); spiwrite(c);
		}

		void SSD_13XX::writedata16_cont(uint16_t d)
		{
			enableDataStream(); spiwrite16(d);
		}

		void SSD_13XX::writecommand_last(const uint8_t c)
		{
			enableCommandStream(); spiwrite(c); disableCS();
		}


		void SSD_13XX::writedata8_last(uint8_t c)
		{
			enableDataStream(); spiwrite(c); disableCS();
		}

		void SSD_13XX::writedata16_last(uint16_t d)
		{
			enableDataStream(); spiwrite16(d); disableCS();
		}

	#endif
#endif
/* ========================================================================
					    Fast Common Graphic Primitives
   ========================================================================*/

void SSD_13XX::drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color)
{
		if (w < 1) return;
		setAddrWindow_cont(x, y, x + w - 1, y,true);
		do { writedata16_cont(color); } while (--w > 0);
}

void SSD_13XX::drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color)
{
		if (h < 1) return;
		setAddrWindow_cont(x, y, x, y + h - 1,true);
		do { writedata16_cont(color); } while (--h > 0);
}

void SSD_13XX::drawPixel_cont(int16_t x, int16_t y, uint16_t color)
{
		setAddrWindow_cont(x, y, x + 1, y + 1,true);
		writedata16_cont(color);
}


void SSD_13XX::drawLine_cont(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0) return;
	#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
		int dly = _dlyHelper(x1-x0,y1-y0,CMD_DLY_LINE);
		uint8_t r,g,b;
		_convertColor(color,r,g,b);
		_sendLineData_cont(x0,y0,x1,y1);
		_sendColor_cont(r,g,b);
		delayMicroseconds(dly);
	#else
		if (y0 == y1) {
			if (x1 > x0) {
				drawFastHLine_cont(x0, y0, x1 - x0 + 1, color);
			} else if (x1 < x0) {
				drawFastHLine_cont(x1, y0, x0 - x1 + 1, color);
			} else {
				drawPixel_cont(x0, y0, color);
			}
			return;
		} else if (x0 == x1) {
			if (y1 > y0) {
				drawFastVLine_cont(x0, y0, y1 - y0 + 1, color);
			} else {
				drawFastVLine_cont(x0, y1, y0 - y1 + 1, color);
			}		
			return;
		}

		bool steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep) {swapVals(x0, y0); swapVals(x1, y1);}
		if (x0 > x1) {swapVals(x0, x1); swapVals(y0, y1);}

		int16_t dx, dy;
		dx = x1 - x0;
		dy = abs(y1 - y0);

		int16_t err = dx / 2;
		int16_t ystep;

		if (y0 < y1) {
			ystep = 1;
		} else {
			ystep = -1;
		}

		int16_t xbegin = x0;
		if (steep) {
			for (; x0<=x1; x0++) {
				err -= dy;
				if (err < 0) {
					int16_t len = x0 - xbegin;
					if (len) {
						drawFastVLine_cont(y0, xbegin, len + 1, color);
					} else {
						drawPixel_cont(y0, x0, color);
					}
					xbegin = x0 + 1;
					y0 += ystep;
					err += dx;
				}
				#if defined(ESP8266)   	
					yield(); 	
				#endif
			}
			if (x0 > xbegin + 1) drawFastVLine_cont(y0, xbegin, x0 - xbegin, color);
		} else {
			for (; x0<=x1; x0++) {
				err -= dy;
				if (err < 0) {
					int16_t len = x0 - xbegin;
					if (len) {
						drawFastHLine_cont(xbegin, y0, len + 1, color);
					} else {
						drawPixel_cont(x0, y0, color);
					}
					xbegin = x0 + 1;
					y0 += ystep;
					err += dx;
				}
				#if defined(ESP8266)   	
					yield(); 	
				#endif
			}
			if (x0 > xbegin + 1) drawFastHLine_cont(xbegin, y0, x0 - xbegin, color);
		}
	#endif
}


#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
	
	void SSD_13XX::_sendLineData_cont(int16_t x0,int16_t y0,int16_t x1,int16_t y1)
	{
		if (_portrait){
			swapVals(x0,y0);
			swapVals(x1,y1);
		}
		writecommand_cont(CMD_DRAWLINE);
		writecommand_cont(x0 & 0xFF);
		writecommand_cont(y0 & 0xFF);
		writecommand_cont(x1 & 0xFF);
		writecommand_cont(y1 & 0xFF);
	}
	
	void SSD_13XX::_sendColor_cont(uint8_t r,uint8_t g,uint8_t b)
	{
		writecommand_cont(r);writecommand_cont(g);writecommand_cont(b);
	}
	
	void SSD_13XX::_sendColor_cont(uint16_t color)
	{
		uint8_t r,g,b;
		_convertColor(color,r,g,b);
		writecommand_cont(r);writecommand_cont(g);writecommand_cont(b);
	}
	
	void SSD_13XX::_fillUtility(bool filling)
	{
		if (filling != _filled){
			_filled = filling;
			writecommand_cont(CMD_FILL);
			if (_filled){
				writecommand_cont(0x01);
			} else {
				writecommand_cont(0x00);
			}
		}
	}

	int SSD_13XX::_dlyHelper(int16_t w,int16_t h,int maxDly)//in microseconds
	{
		if (w <= 0 || h <= 0) return CMD_DLY_MIN;
		return map(w*h,2,SSD_CGRAM,CMD_DLY_MIN,maxDly);
	}

	void SSD_13XX::_convertColor(uint16_t color,uint8_t &r,uint8_t &g,uint8_t &b)
	{
		r = (uint8_t)((color >> 11) << 1);
		g =	(uint8_t)((color >> 5) & 0x3F);
		b = (uint8_t)((color << 1) & 0x3F);
	}
		
	/*
	void SSD_13XX::_convertColor(uint16_t color,uint8_t &r,uint8_t &g,uint8_t &b)
	{
		if (_colorDepth){
			//16 bit
			r = (uint8_t)((color >> 11) << 1);
			g =	(uint8_t)((color >> 5) & 0x3F);
			b = (uint8_t)((color << 1) & 0x3F);
		} else {
			//8 bit
			r = (uint8_t)((color >> 5) & 0x07);
			r |= r << 3;
			g = (uint8_t)((color >> 2) & 0x07);
			g |= g << 3;
			b = (uint8_t)(color & 0x03);
			b |= (b << 4) | (b << 2);
		}
	}
	*/
#endif

	//+++++++++OK
	void SSD_13XX::drawRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2, bool filled)
	{
		#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
			if (w < 2 && h < 2) {
				drawPixel_cont(x,y,color1);
				return;
			}
			if (_portrait){
				swapVals(x, y);
				swapVals(w, h);
			}
			if (x >= _width || y >= _height) return;
			int dly = _dlyHelper(w,h,CMD_DLY_FILL);

			uint8_t r1,g1,b1,r2,g2,b2;
			_convertColor(color1,r1,g1,b1);
			if (color1 != color2){
				_convertColor(color2,r2,g2,b2);
			} else {
				r2 = r1;g2 = g1;b2 = b1;
			}
			_fillUtility(filled);
			writecommand_cont(CMD_DRAWRECT);
			writecommand_cont(x & 0xFF);
			writecommand_cont(y & 0xFF);
			if ((x + w - 1) >= SSD_WIDTH) {
				writecommand_cont((SSD_WIDTH-1) & 0xFF);
			} else {
				writecommand_cont(((x + w) - 1) & 0xFF);
			}
			if ((y + h - 1) >= SSD_HEIGHT) {
				writecommand_cont((SSD_HEIGHT-1) & 0xFF);
			} else {
				writecommand_cont(((y + h) - 1) & 0xFF);
			}
			_sendColor_cont(r1,g1,b1);
			_sendColor_cont(r2,g2,b2);
			delayMicroseconds(dly);
		#else
			if (filled){
				fillRect_cont(x,y,w,h,color1,color2);
			} else {
				//TODO Grandient? (and color2)
				drawFastHLine_cont(x, y, w, color1);
				drawFastHLine_cont(x, (y+h)-1, w, color1);
				drawFastVLine_cont(x, y, h, color1);
				drawFastVLine_cont((x+w)-1, y, h, color1);	
			}
		#endif
	}

	void SSD_13XX::setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,bool rotFix)
	{
		if (rotFix && _portrait){
			swapVals(x0, y0);
			swapVals(x1, y1);
		}
		#if defined(_SSD_1331_REG_H_) || defined(_SSD_1332_REG_H_)
			writecommand_cont(CMD_SETCOLUMN); //Column
			writecommand_cont(x0); writecommand_cont(x1);
			writecommand_cont(CMD_SETROW); //Page
			writecommand_cont(y0); writecommand_cont(y1);
		#else
			writecommand_cont(CMD_SETCOLUMN); //Column
			writedata8_cont(x0); writedata8_cont(x1);
			writecommand_cont(CMD_SETROW); //Page
			writedata8_cont(y0); writedata8_cont(y1);
			
			writecommand_cont(CMD_WRITERAM);
		#endif
	}

	void SSD_13XX::setAddrWindow_cont(uint16_t x, uint16_t y)
	{
		setAddrWindow_cont(x,y,SSD_WIDTH,SSD_HEIGHT,false);
	}

	bool SSD_13XX::boundaryCheck(int16_t xw,int16_t yh)
	{
		if ((xw >= _width) || (yh >= _height)) return true;
		return false;
	}

	int16_t SSD_13XX::sizeCheck(int16_t origin,int16_t len,int16_t maxVal)
	{
		if (((origin + len) - 1) >= maxVal) len = maxVal - origin;
		return len;
	}


	void SSD_13XX::closeTransaction(void)
	{
		#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
			writecommand_last(CMD_NOP);
		#else
			disableCS();
		#endif
		endTransaction();
	}

/*
void SSD_13XX::printPacket(word data,uint8_t count){
  for (int i=count-1; i>=0; i--){
    if (bitRead(data,i)==1){
      Serial.print("1");
    }
    else {
      Serial.print("0");
    }
  }
  Serial.print(" -> 0x");
  if (count == 8){
	  Serial.print((byte)data,HEX);
  } else {
	  Serial.print(data,HEX);
  }
  Serial.print("\n");
}
*/





/*
  uint16_t r,g,b;
  if(_bitDepth){
    r=(color)&0x1F;//five bits
    g=(color>>5)&0x3F;//six bits
    b=(color>>11)&0x1F;//five bits
    r=r<<1;//shift to fill six bits
    g=g<<0;//shift to fill six bits
    b=b<<1;//shift to fill six bits
  }else{
    r=(color)&0x03;//two bits
    g=(color>>2)&0x07;//three bits
    b=(color>>5)&0x07;//three bits
    r|=(r<<4)|(r<<2);//copy to fill six bits
    g|=g<<3;//copy to fill six bits
    b|=b<<3;//copy to fill six bits
}
*/



