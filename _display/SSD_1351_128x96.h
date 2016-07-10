#ifndef _SSD_1351_128X96_H
#define _SSD_1351_128X96_H
#include <stdio.h>
#include "../_includes/SSD_1351_registers.h"

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
	Display Type: SSD_1351 128 x 96 (never tested)
===========================================================================================*/
	/* Physical display size
	*/
	static const int16_t	SSD_WIDTH				=	128;
	static const int16_t	SSD_HEIGHT 				=	96;


	static const int16_t	SSD_COMSPLIT 			=	1;			//this unit needs com split
	static const uint8_t	SSD_COLORDEPTH  		= 	16;			//8 or 16
	static const uint8_t	SSD_RGBORDER			= 	1;			//0:RGB 1:
	static const bool		SSD_ENHANCE				= 	1;
	static const uint8_t	SSD_COMMANDLOCK1		=	0x12;
	static const uint8_t	SSD_COMMANDLOCK2		=	0xB1;
	static const uint8_t	SSD_MUXRATIO			=	0x7F;
	static const uint8_t	SSD_SETGPIO				=	0x00;
	static const uint8_t	SSD_PRECHARGE2			=	0x01;
	static const uint8_t	SSD_CONTRASTMASTER		=	0x0F;
	static const uint8_t	SSD_FUNCTIONSELECT		=	0x01;
	static const uint8_t	SSD_DISPLAYOFFSET		=	0x00;
	static const uint8_t	SSD_PHASEPERIOD			=	0b10110001;
	static const uint8_t	SSD_SETMULTIPLEX		=	0x3F;
	static const uint8_t	SSD_STARTLINE			=	SSD_HEIGHT;
	static const uint8_t	SSD_FILL				=	0x01;
	static const uint8_t	SSD_SETMASTER			=	0x8E;
	static const uint8_t	SSD_POWERMODE			=	0x0B;
	static const uint8_t	SSD_PRECHARGE			=	0x32;
	static const uint8_t	SSD_CLOCKDIV			=	0xF1;
	static const uint8_t	SSD_SETVSL_A			=	0xA0;
	static const uint8_t	SSD_SETVSL_B			=	0xB5;
	static const uint8_t	SSD_SETVSL_C			=	0x55;
	static const uint8_t	SSD_PRECHARGELEVEL		=	0x3A;
	static const uint8_t	SSD_VCOMH				=	0x05;
	static const uint8_t	SSD_MASTERCURRENT		=	0x0F;//0x06
	static const uint8_t	SSD_CONTRAST_A			=	0xC8;
	static const uint8_t	SSD_CONTRAST_B			=	0x80;
	static const uint8_t	SSD_CONTRAST_C			=	0xC8;

#endif