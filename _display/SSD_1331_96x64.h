#ifndef _SSD_1331_96X64_H
#define _SSD_1331_96X64_H
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
	Display Type: SSD_1331 96x64
===========================================================================================*/
	/* Physical display size
	*/
	static const int16_t	SSD_WIDTH	=	96;
	static const int16_t	SSD_HEIGHT 	=	64;
	
	static const int	 CMD_DLY_LINE			= 200;//SSD1351 400
	static const int	 CMD_DLY_FILL			= 400;//SSD1351 800
	static const int	 CMD_DLY_MIN			= 4;//this is the min delay (nicrosec) for this device
	
	/* Gamma Set*/
	#define SSD_GAMMASET

	static const int16_t	SSD_COMSPLIT 			=	1;			//this unit needs com split
	static const uint8_t	SSD_COLORDEPTH  		= 	16;			//8 or 16
	static const uint8_t	SSD_RGBORDER			= 	0;			//0:RGB 1:
	static const uint8_t	SSD_DISPLAYOFFSET		=	0x00;
	static const uint8_t	SSD_PHASEPERIOD			=	0b10110001;
	static const uint8_t	SSD_SETMULTIPLEX		=	0x3F;
	static const uint8_t	SSD_STARTLINE			=	0x00;
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
	static const uint8_t	SSD_VPACOLORLVL			=	0b00111111;
	static const uint8_t	SSD_VPBCOLORLVL			=	0b00111111;
	static const uint8_t	SSD_VPCCOLORLVL			=	0b00111111;
	static const uint8_t	SSD_DIMMDESET_A			=	128;
	static const uint8_t	SSD_DIMMDESET_B			=	128;
	static const uint8_t	SSD_DIMMDESET_C			=	128;
	static const uint8_t	SSD_DIMMDESET_PC		=	0x0F;//max 0x1F

	#if defined(SSD_GAMMASET)
	static const uint8_t 	SSD_GRAYTABLE[32]= 	{0x01,0x03,0x05,0x07,0x0A,0x0D,0x10,0x13,0x16,0x19,0x1C,0x20,0x24,0x28,0x2C,0x30,0x34,0x38,0x3C,0x40,0x44,0x48,0x4C,0x50,0x54,0x58,0x5C,0x60,0x64,0x68,0x6C,0x70};
	#endif
#endif