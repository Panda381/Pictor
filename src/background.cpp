
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "Include.h"

// background
int BackInx;		// index of background
int BackPhase1, BackPhase2, BackPhase3; // background phases

// background images
SCanvas* ImgBg1[BG_NUM];
SCanvas* ImgBg2[BG_NUM];
SCanvas* ImgBg3[BG_NUM];

// background name
const char* BackName[BG_NUM] = {
	"Meadow",
	"Jungle",
	"Sandy Beach",
	"Underwater",
	"Ice Land",
	"Fiery Land",
	"Haunted Hill",
	"Spacecraft",
	"Galaxy",
	"Alien Planet",
	"Candy Land",
	"Surreal Land",
};

// load backgrounds on program start
BOOL LoadBack()
{
	if (!LoadImg(&ImgBg1[0], TGA_01BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[1], TGA_02BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[2], TGA_03BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[3], TGA_04BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[4], TGA_05BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[5], TGA_06BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[6], TGA_07BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[7], TGA_08BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[8], TGA_09BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[9], TGA_10BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[10], TGA_11BG1)) return FALSE;
	if (!LoadImg(&ImgBg1[11], TGA_12BG1)) return FALSE;

	if (!LoadImg(&ImgBg2[0], TGA_01BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[1], TGA_02BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[2], TGA_03BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[3], TGA_04BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[4], TGA_05BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[5], TGA_06BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[6], TGA_07BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[7], TGA_08BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[8], TGA_09BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[9], TGA_10BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[10], TGA_11BG2)) return FALSE;
	if (!LoadImg(&ImgBg2[11], TGA_12BG2)) return FALSE;

	if (!LoadImg(&ImgBg3[0], TGA_01BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[1], TGA_02BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[2], TGA_03BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[3], TGA_04BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[4], TGA_05BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[5], TGA_06BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[6], TGA_07BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[7], TGA_08BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[8], TGA_09BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[9], TGA_10BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[10], TGA_11BG3)) return FALSE;
	if (!LoadImg(&ImgBg3[11], TGA_12BG3)) return FALSE;

	return True;
}

// initialize backgrounds on new level
void InitBack()
{
	// prepare background template
	BackInx = Level % BG_NUM;

	// clear background phase
	BackPhase1 = 0;
	BackPhase2 = 0;
	BackPhase3 = 0;
}

// display backgrounds
void DispBack()
{
	// draw background 1
	SCanvas* img = ImgBg1[BackInx];
	if (BackPhase1 < BG_WIDTH - WIDTH)
	{
		DrawImage(img, 				// image
			BackPhase1, 0,			// source X, Y
			0, BG_Y,				// destination X, Y
			WIDTH, BG_HEIGHT);		// width, height
	}
	else
	{
		DrawImage(img,				// image
			BackPhase1, 0,			// source X, Y
			0, BG_Y,				// destination X, Y
			BG_WIDTH - BackPhase1, BG_HEIGHT); // width, height

		DrawImage(img, 				// image
			0, 0,					// source X, Y
			BG_WIDTH - BackPhase1, BG_Y, // destination X, Y
			WIDTH - (BG_WIDTH - BackPhase1), BG_HEIGHT); // width, height
	}

	// draw background 2
	img = ImgBg2[BackInx];
	int h = img->h;
	if (BackPhase2 < BG2_WIDTH - WIDTH)
	{
		DrawImageTrans(img,			// image
			BackPhase2, 0,			// source X, Y
			0, BG2_Y+BG2_HEIGHT-h,	// destination X, Y
			WIDTH, h);				// width, height
	}
	else
	{
		DrawImageTrans(img,			// image
			BackPhase2, 0,			// source X, Y
			0, BG2_Y+BG2_HEIGHT-h,	// destination X, Y
			BG2_WIDTH - BackPhase2, h); // width, height

		DrawImageTrans(img,			// image
			0, 0,					// source X, Y
			BG2_WIDTH - BackPhase2, BG2_Y+BG2_HEIGHT-h,	// destination X, Y
			WIDTH - (BG2_WIDTH - BackPhase2), h); // width, height
	}

	// draw background 3
	img = ImgBg3[BackInx];
	if (BackPhase3 < BG3_WIDTH - WIDTH)
	{
		DrawImageTrans(img,			// image
			BackPhase3, 0,			// source X, Y
			0, BG3_Y,				// destination X, Y
			WIDTH, BG3_HEIGHT);		// width, height
	}
	else
	{
		DrawImageTrans(img,			// image data and palettes
			BackPhase3, 0,			// source X, Y
			0, BG3_Y,				// destination X, Y
			BG3_WIDTH - BackPhase3, BG3_HEIGHT); // width, height

		DrawImageTrans(img,			// image
			0, 0,					// source X, Y
			BG3_WIDTH - BackPhase3, BG3_Y, // destination X, Y
			WIDTH - (BG3_WIDTH - BackPhase3), BG3_HEIGHT); // width, height
	}
}

// shift backgrounds
void ShiftBack()
{
	// background layer 1
	BackPhase1 += BG_SPEED1;
	if (BackPhase1 >= BG_WIDTH) BackPhase1 -= BG_WIDTH;

	// background layer 2
	BackPhase2 += BG_SPEED2;
	if (BackPhase2 >= BG2_WIDTH) BackPhase2 -= BG2_WIDTH;

	// background layer 3
	BackPhase3 += BG_SPEED3;
	if (BackPhase3 >= BG3_WIDTH) BackPhase3 -= BG3_WIDTH;
}
