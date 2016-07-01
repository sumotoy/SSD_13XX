#ifndef _SSD_1331_96x64P_H
#define _SSD_1331_96x64P_H
#include <stdio.h>
#include "../_includes/SSD_1331_registers.h"

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
	Single Display Parameters
	---------------------------------------------------------------------------------------
	Display Type: SSD1331 96 x 64
===========================================================================================*/
	/* Physical display size
	*/
	static const int16_t	SSD_WIDTH	=	96;
	static const int16_t	SSD_HEIGHT 	=	64;
	//static const uint32_t	SSD_CGRAM		=	SSD_WIDTH * SSD_HEIGHT;
	/* Gamma Set*/
	#define SSD_GAMMASET
	/* Colorspace
	Some display can use GBR colorspace or RGB (1:GBR - 0:RGB) */
	//static const uint8_t	SSD_CSPACE 		= 	MADCTL_RGB;
	static const uint8_t	SSD_CSPACE[4][2]	=	{
		{0x72,0x76},				//rot0(0)
			{0x71,0x75},			//rot1(90)
				{0x60,0x64},		//rot2(180)
					{0x63,0x67}		//rot3(270)
	};
	static const uint8_t	SSD_RGBORDER	= 0;//RGB
	/* Offset
	Vendors like to set his display as they like, result in offset that is different between models
	Here's a offset map for any rotatio of the display, helping code to deal with this.
	First index is rotation, second 2 parameters are offset in x and y axis. [rotation][x,y] */
	
	
	//#define SSD_OFSTNEED
	
	#if defined(SSD_OFSTNEED)
	static const uint8_t 	SSD_OFST[4][2] = {
		{0,0},//rot 0 - x,y
		{0,0},//rot 1 - x,y
		{0,0},//rot 2 - x,y
		{0,0} //rot 3 - x,y
	};
	#endif

	//Initialization Parameters
	static const uint8_t	SSD_DISPLAYOFFSET		=	0x00;
	static const uint8_t	SSD_PHASEPERIOD			=	0b10110001;
	static const uint8_t	SSD_SETMULTIPLEX		=	0x3F;
	static const uint8_t	SSD_STARTLINE			=	0x00;
	static const uint8_t	SSD_FILL				=	0x01;
	static const uint8_t	SSD_SETMASTER			=	0x8E;
	static const uint8_t	SSD_POWERMODE			=	0x0B;
	static const uint8_t	SSD_PRECHARGE			=	0x31;
	static const uint8_t	SSD_CLOCKDIV			=	0xF0;
	static const uint8_t	SSD_PRECHARGE_A			=	0x64;
	static const uint8_t	SSD_PRECHARGE_B			=	0x78;
	static const uint8_t	SSD_PRECHARGE_C			=	0x64;
	static const uint8_t	SSD_PRECHARGELEVEL		=	0x3A;
	static const uint8_t	SSD_VCOMH				=	0x3E;
	static const uint8_t	SSD_MASTERCURRENT		=	0x06;
	static const uint8_t	SSD_CONTRAST_A			=	0x91;
	static const uint8_t	SSD_CONTRAST_B			=	0x50;
	static const uint8_t	SSD_CONTRAST_C			=	0x7D;

	static const uint8_t 	SSD_GRAYTABLE[32]= 	{0x01,0x03,0x05,0x07,0x0A,0x0D,0x10,0x13,0x16,0x19,0x1C,0x20,0x24,0x28,0x2C,0x30,0x34,0x38,0x3C,0x40,0x44,0x48,0x4C,0x50,0x54,0x58,0x5C,0x60,0x64,0x68,0x6C,0x70};

#endif