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
	SSD1351 Registers set
===========================================================================================*/
#ifndef _SSD_1351_REG_H_
#define _SSD_1351_REG_H_

//registers-----------------------
static const uint8_t CMD_NOP     			= 0xAD;//B0//D1//E3
static const uint8_t CMD_WRITERAM			= 0x5C;
static const uint8_t CMD_READRAM			= 0x5D;
static const uint8_t CMD_FUNCTIONSELECT		= 0xAB;
static const uint8_t CMD_DISPLAYENHANCE		= 0xB2;
static const uint8_t CMD_SETVSL				= 0xB4;
static const uint8_t CMD_SETGPIO			= 0xB5;
static const uint8_t CMD_PRECHARGE2			= 0xB6;
static const uint8_t CMD_USELUT				= 0xB9;
static const uint8_t CMD_CONTRASTABC		= 0xC1;
static const uint8_t CMD_MASTERCURRENT		= 0xC7;
static const uint8_t CMD_MUXRATIO			= 0xCA;
static const uint8_t CMD_CMDLOCK			= 0xFD;
static const uint8_t CMD_HORIZSCROLL		= 0x96;
static const uint8_t CMD_SCROLL_OFF 		= 0x9E;//SSD1351
static const uint8_t CMD_SCROLL_ON  		= 0x9F;//SSD1351
static const uint8_t CMD_STARTLINE 			= 0xA1;//SSD1351
static const uint8_t CMD_DISPLAYALLON 		= 0xA5;//SSD1351
static const uint8_t CMD_DISPLAYALLOFF 		= 0xA6;//SSD1351
//static const uint8_t CMD_DINVOF  			= 0x20;//Display Inversion OFF
//static const uint8_t CMD_PHASEPERIOD 		= 0x12;
//static const uint8_t CMD_CONTRASTA 			= 0x81;
//static const uint8_t CMD_CONTRASTB 			= 0x82;
//static const uint8_t CMD_CONTRASTC 			= 0x83;
//static const uint8_t CMD_MASTERCURRENT 		= 0x87;
static const uint8_t CMD_SETREMAP 			= 0xA0;//SSD1351
static const uint8_t CMD_DISPLAYOFFSET 		= 0xA2;//SSD1351
//static const uint8_t CMD_SETMULTIPLEX 		= 0xA8;
//static const uint8_t CMD_SETMASTER			= 0xAD;
//static const uint8_t CMD_POWERMODE 			= 0xB0;
static const uint8_t CMD_PRECHARGE 			= 0xB1;//SSD1351
static const uint8_t CMD_CLOCKDIV 			= 0xB3;//SSD1351
static const uint8_t CMD_GRAYSCALE 			= 0xB8;
static const uint8_t CMD_PRECHARGELEVEL 	= 0xBB;//SSD1351
static const uint8_t CMD_VCOMH 				= 0xBE;//SSD1351
static const uint8_t CMD_NORMALDISPLAY   	= 0xA6;//SSD1351
static const uint8_t CMD_INVERTDISPLAY   	= 0xA7;//SSD1351
static const uint8_t CMD_DISPLAYOFF 		= 0xAE;//SSD1351
static const uint8_t CMD_DISPLAYON  		= 0xAF;//SSD1351
static const uint8_t CMD_SETCOLUMN   		= 0x15;//SSD1351
static const uint8_t CMD_SETROW   			= 0x75;//SSD1351
static const uint8_t CMD_SLEEPON   			= 0xAE;//SSD1351
static const uint8_t CMD_SLEEPOFF   		= 0xAF;//SSD1351
//static const uint8_t CMD_VSSTADRS	= 0x37;//Vertical Scrolling Start address				_CMD_VSCLLSA

/*
----------------------------------------------------------------------------------	 
REMAP 01 1 1 0 1 0 0
7,6) 00:256colors 								*01:65K  						10:256k
5)	 0:disable com split						*1:enable com split
4)	 0:Scan from up to down 					1:Scan from bottom to up 
3)	 reserved
2)	 0:RGB										1:GBR										RGB
1)	 0:RAM Column 0 ~ 127 maps to Col0~Col127	1:RAM Column 0 ~ 127 maps to Col127~Col0 (Column Address Remap)
0)	 0:Hor Address increment					1:Ver Address increment
*/


#endif