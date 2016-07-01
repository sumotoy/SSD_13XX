#ifndef _SSD_1331_REG_H_
#define _SSD_1331_REG_H_

//registers-----------------------
static const uint8_t CMD_NOP     			= 0xE3;//***Non operation
static const int	 CMD_DLY_LINE			= 400;//SSD1351
static const int	 CMD_DLY_FILL			= 800;//SSD1351
static const uint8_t CMD_DRAWLINE 			= 0x21;
static const uint8_t CMD_DRAWRECT 			= 0x22;
static const uint8_t CMD_DRAWCOPY 			= 0x23;
static const uint8_t CMD_DIMWINDOW 			= 0x24;
static const uint8_t CMD_CLRWINDOW 			= 0x25;
static const uint8_t CMD_FILL 				= 0x26;
static const uint8_t CMD_SCROLL_OFF 		= 0x2E;//SSD1351
static const uint8_t CMD_SCROLL_ON  		= 0x2F;//SSD1351
static const uint8_t CMD_STARTLINE 			= 0xA1;//SSD1351
static const uint8_t CMD_DISPLAYALLON 		= 0xA5;//SSD1351
static const uint8_t CMD_DISPLAYALLOFF 		= 0xA6;//SSD1351
static const uint8_t CMD_DINVOF  			= 0x20;//Display Inversion OFF
static const uint8_t CMD_PHASEPERIOD 		= 0x12;
static const uint8_t CMD_CONTRASTA 			= 0x81;
static const uint8_t CMD_CONTRASTB 			= 0x82;
static const uint8_t CMD_CONTRASTC 			= 0x83;
static const uint8_t CMD_MASTERCURRENT 		= 0x87;
static const uint8_t CMD_SETREMAP 			= 0xA0;//SSD1351
static const uint8_t CMD_DISPLAYOFFSET 		= 0xA2;//SSD1351
static const uint8_t CMD_SETMULTIPLEX 		= 0xA8;
static const uint8_t CMD_SETMASTER			= 0xAD;
static const uint8_t CMD_POWERMODE 			= 0xB0;
static const uint8_t CMD_PRECHARGE 			= 0xB1;//SSD1351
static const uint8_t CMD_CLOCKDIV 			= 0xB3;//SSD1351
static const uint8_t CMD_GRAYSCALE 			= 0xB8;
static const uint8_t CMD_PRECHARGEA 		= 0x8A;
static const uint8_t CMD_PRECHARGEB			= 0x8B;
static const uint8_t CMD_PRECHARGEC 		= 0x8C;
static const uint8_t CMD_PRECHARGELEVEL 	= 0xBB;//SSD1351
static const uint8_t CMD_VCOMH 				= 0xBE;//SSD1351
static const uint8_t CMD_VPACOLORLVL 		= 0xBB;
static const uint8_t CMD_VPBCOLORLVL 		= 0xBC;
static const uint8_t CMD_VPCCOLORLVL 		= 0xBD;
static const uint8_t CMD_NORMALDISPLAY   	= 0xA4;//SSD1351
static const uint8_t CMD_INVERTDISPLAY   	= 0xA7;//SSD1351
static const uint8_t CMD_DISPLAYOFF 		= 0xAE;//SSD1351
static const uint8_t CMD_DISPLAYON  		= 0xAF;//SSD1351
static const uint8_t CMD_SETCOLUMN   		= 0x15;//SSD1351
static const uint8_t CMD_SETROW   			= 0x75;//SSD1351


static const uint8_t CMD_VSCLLDEF	= 0x33;//Vertical Scroll Definition						_CMD_VSCRLLD
static const uint8_t CMD_VSSTADRS	= 0x37;//Vertical Scrolling Start address				_CMD_VSCLLSA


/*
----------------------------------------------------------------------------------	 
	 
     MY, MX, MV, ML, MH, RGB,D1, D0
	 0 | 0 | 0 | 0 | 1 | 0 | x | x	//normal
	 1 | 0 | 0 | 0 | 1 | 0 | x | x	//Y-Mirror
	 0 | 1 | 0 | 0 | 1 | 0 | x | x	//X-Mirror
	 1 | 1 | 0 | 0 | 1 | 0 | x | x	//X-Y-Mirror
	 0 | 0 | 1 | 0 | 1 | 0 | x | x	//X-Y Exchange
	 1 | 0 | 1 | 0 | 1 | 0 | x | x	//X-Y Exchange, Y-Mirror
	 0 | 1 | 1 | 0 | 1 | 0 | x | x	//XY exchange
	 1 | 1 | 1 | 0 | 1 | 0 | x | x
	 
	0b01000000 
7,6) 00:256colors 					01:65K  						10:65k2
5)	 0:disable com split			1:enable com split
4)	 0:scan from COM0 to COM(n-1)	1:scan from COM(n-1) to COM0
3)	 0:disable Left/Right swap		1:enable Left/Right swap					MH?
2)	 0:RGB							1:GBR										RGB
1)	 0:RAM col 0->95				1:RAM col 95->0
0)	 0:Hor Address increment		1:Ver Address increment
*/

static const uint8_t SSD_COLSPACE	=	0x40;
static const uint8_t SSD_COMSPLIT	=	0x20;
static const uint8_t SSD_INVSCAN	=	0x10;
static const uint8_t SSD_LRSWAP		=	0x08;
static const uint8_t SSD_GBR		=	0x04;
static const uint8_t SSD_COLSWAP	=	0x02;
static const uint8_t SSD_VERADRSINC	=	0x01;
#endif