#ifndef _SSD_1332_REG_H_
#define _SSD_1332_REG_H_
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
	SSD1332 Registers set
===========================================================================================*/
#define _SSD_USECMDASDATA 1
//registers-----------------------
static const uint8_t CMD_NOP     			= 0xE3;//SSD1332
static const uint8_t CMD_DRAWLINE 			= 0x21;//SSD1332
static const uint8_t CMD_DRAWRECT 			= 0x22;//SSD1332
static const uint8_t CMD_DRAWCOPY 			= 0x23;//SSD1332
static const uint8_t CMD_DIMWINDOW 			= 0x24;//SSD1332
static const uint8_t CMD_CLRWINDOW 			= 0x25;//SSD1332
static const uint8_t CMD_FILL 				= 0x26;//SSD1332
static const uint8_t CMD_SCROLL_OFF 		= 0x2E;//
static const uint8_t CMD_SCROLL_ON  		= 0x2F;//
static const uint8_t CMD_SCROLL_SET  		= 0x2F;
static const uint8_t CMD_STARTLINE 			= 0xA1;//SSD1332
static const uint8_t CMD_DISPLAYALLON 		= 0xA5;//SSD1332
static const uint8_t CMD_DISPLAYALLOFF 		= 0xA6;//SSD1332
//static const uint8_t CMD_DINVOF  			= 0x20;
static const uint8_t CMD_PHASEPERIOD 		= 0x12;//SSD1332
static const uint8_t CMD_CONTRASTA 			= 0x81;//SSD1332
static const uint8_t CMD_CONTRASTB 			= 0x82;//SSD1332
static const uint8_t CMD_CONTRASTC 			= 0x83;//SSD1332
//static const uint8_t CMD_DIMMODESET 		= 0xAB;
static const uint8_t CMD_MASTERCURRENT 		= 0x87;//SSD1332
static const uint8_t CMD_SETREMAP 			= 0xA0;//SSD1332
static const uint8_t CMD_DISPLAYOFFSET 		= 0xA2;//SSD1332
static const uint8_t CMD_SETMULTIPLEX 		= 0xA8;//SSD1332
static const uint8_t CMD_SETMASTER			= 0xAD;//SSD1332
static const uint8_t CMD_POWERMODE 			= 0xB0;//SSD1332
//static const uint8_t CMD_PRECHARGE 			= 0xB1;//SSD1332
static const uint8_t CMD_CLOCKDIV 			= 0xB3;//SSD1332
static const uint8_t CMD_GRAYSCALE 			= 0xB8;//SSD1332
//static const uint8_t CMD_PRECHARGEA 		= 0x8A;//SSD1332
//static const uint8_t CMD_PRECHARGEB			= 0x8B;//SSD1332
//static const uint8_t CMD_PRECHARGEC 		= 0x8C;//SSD1332
static const uint8_t CMD_LINEARGRAY 		= 0xB9;//SSD1332
//static const uint8_t CMD_PRECHARGELEVEL 	= 0xBB;//SSD1332
static const uint8_t CMD_VCOMH 				= 0xBE;//SSD1332
static const uint8_t CMD_VPACOLORLVL 		= 0xBB;//SSD1332
static const uint8_t CMD_VPBCOLORLVL 		= 0xBC;//SSD1332
static const uint8_t CMD_VPCCOLORLVL 		= 0xBD;//SSD1332
//static const uint8_t CMD_CMDLOCK	 		= 0xFD;
static const uint8_t CMD_NORMALDISPLAY   	= 0xA4;//SSD1332
static const uint8_t CMD_INVERTDISPLAY   	= 0xA7;//SSD1332
static const uint8_t CMD_DISPLAYOFF 		= 0xAE;//SSD1332
static const uint8_t CMD_DISPLAYDIM 		= 0xAC;
static const uint8_t CMD_DISPLAYON  		= 0xAF;//SSD1332
static const uint8_t CMD_SETCOLUMN   		= 0x15;//SSD1332
static const uint8_t CMD_SETROW   			= 0x75;//SSD1332



/* The remap command
1332
7,6) 00:256colors 					01:65K
5)	 0:disable com split			1:enable com split
4)	 0:scan from COM0 to COM(n-1)	1:scan from COM(n-1) to COM0
3)	 reserved <-- Argh!
2)	 reserved
1)	 0:RAM col 0->95				1:RAM col 95->0
0)	 0:Hor Address increment		1:Ver Address increment
*/

#endif