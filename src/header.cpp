
// ****************************************************************************
//
//                                  Header
//
// ****************************************************************************

#include "Include.h"

// current and max score
int Score = 0;
int MaxScore = 0;

// decode buffer
char DecNumBuf[15];

// display current score
void DispScore()
{
	int len = sprintf(DecNumBuf, "%07d", Score);
	DrawTextBg(DecNumBuf, (6*8+2)*SCALE, TIT_Y+2*SCALE, (Score > MaxScore) ? COL_GREEN : COL_WHITE, HEADER_COL);
}

// display max. score
void DispMaxScore()
{
	int len = sprintf(DecNumBuf, "%07d", (Score > MaxScore) ? Score : MaxScore);
	DrawTextBg(DecNumBuf, WIDTH-(7*8+2)*SCALE, TIT_Y+2*SCALE, (Score > MaxScore) ? COL_GREEN : COL_WHITE, HEADER_COL);
}

// display Picopad
void DispPico()
{
	int mask = BonusMask;

	// Picopad is complete
	if (mask == BONUS_MASK_ALL)
	{
		SCanvas* img = ImgBonus[BONUS_ON];
		int w = img->w;
		int h = img->h;
		DrawImageTrans(img,	// image data and palettes
			0, 0,					// source X, Y
			(WIDTH - w - (BONUS_BIGSCORE_LEN+1)*8*SCALE)/2, TIT_Y + (TIT_HEIGHT - h)/2, // destination X, Y
			w, h);			// width, height

		// display points
		DrawText(BONUS_BIGSCORE_TEXT,
			(WIDTH - w - (BONUS_BIGSCORE_LEN+1)*8*SCALE)/2 + w + 8*SCALE, TIT_Y + 2*SCALE,
			COL_YELLOW);
	}
	else
	{
		int x = (6*8+2 + 7*8+2 + 15)*SCALE;
		int i = BONUS_COLFIRST;
		for (; i <= BONUS_COLLAST; i++)
		{
			SCanvas* img = ImgBonus[i];
			if ((mask & BIT(i)) != 0)
			{
				DrawImageTrans(img,			// image data and palettes
					0, 0,					// source X, Y
					x, TIT_Y + (TIT_HEIGHT - img->h)/2, // destination X, Y
					img->w, img->h);		// width, height
				x += img->w + 2*SCALE;
			}
		}
	}
}

// display header
void DispHeader()
{
	// background
	DrawRect(SCALE, TIT_Y+SCALE, WIDTH-2*SCALE, TIT_HEIGHT-2*SCALE, HEADER_COL);

	// frame
	DrawFrame(0, TIT_Y, WIDTH, TIT_HEIGHT, SCALE, HEADER_FRAMECOL);

	// title SCORE
	DrawText("SCORE", 2*SCALE, TIT_Y+2*SCALE, COL_YELLOW);

	// title MAX
	DrawText("MAX", WIDTH-(11*8+2)*SCALE, TIT_Y+2*SCALE, COL_YELLOW);

	// display current score
	DispScore();

	// display max. score
	DispMaxScore();

	// display Picopad
	DispPico();
}
