
// ****************************************************************************
//
//                                 Bonus
//
// ****************************************************************************

#include "Include.h"

// bonus images
SCanvas* ImgBonus[BONUS_NUM];

// bonuses in levels
int BonusLevel[BG_NUM] = {
	BONUS_TOP,			// 1 Meadow
	BONUS_CPU,			// 2 Jungle
	BONUS_HEART,		// 3 Sandy Beach
	BONUS_DISP,			// 4 Underwater
	BONUS_BAT,			// 5 Ice Land
	BONUS_HEART,		// 6 Fiery Land
	BONUS_TOP,			// 7 Haunted Hill
	BONUS_CPU,			// 8 Spacecraft
	BONUS_HEART,		// 9 Galaxy
	BONUS_DISP,			// 10 Alien Planet
	BONUS_BAT,			// 11 Candy Land
	BONUS_HEART,		// 12 Surreal Land
};

// current bonus
int BonusMask;				// mask of collected Picopad components (BONUS_MASK_*)
int BonusInx;				// index of current bonus (-1 if bonus is not active)
SCanvas* Bonus;				// current bonus template
int BonusX;					// coordinate X of current bonus
int BonusY;					// coordinate Y of current bonus
int BonusPhase;				// bonus animation phase (0..2)
SCanvas* BonuscloudImg;		// bonus cloud image
int BonuscloudW;			// width of bonus cloud image

// load bonus images
BOOL LoadBonus()
{
	if (!LoadImg(&ImgBonus[0], TGA_HEART)) return FALSE;
	if (!LoadImg(&ImgBonus[1], TGA_PICOPAD1)) return FALSE;
	if (!LoadImg(&ImgBonus[2], TGA_PICOPAD2)) return FALSE;
	if (!LoadImg(&ImgBonus[3], TGA_PICOPAD3)) return FALSE;
	if (!LoadImg(&ImgBonus[4], TGA_PICOPAD4)) return FALSE;
	if (!LoadImg(&ImgBonus[5], TGA_PICOPAD)) return FALSE;
	if (!LoadImg(&BonuscloudImg, TGA_BONUSCLOUD)) return FALSE;
	BonuscloudW = BonuscloudImg->w/BONUS_PHASES;

	return TRUE;
}

// initialize bonus on start of next level (requires initialized EnemyRandSeed)
void InitBonus()
{
	BonusInx = BonusLevel[BackInx];	// bonus index
	Bonus = ImgBonus[BonusInx];	// bonus template
	BonusX = MaxFrame/2*BG_SPEED3*SCALE + WIDTH;	// X coordinate
	BonusY = EnemyRandMinMax(BG_Y_MIN + 100*SCALE, BG_Y_MAX - 40*SCALE); // random Y coordiate
	BonusPhase = 0;			// bonus animation phase
}

// shift bonus
void BonusShift()
{
	// shift animation phase
	if (SubFrame == 0)
	{
		BonusPhase++;
		if (BonusPhase >= BONUS_PHASES) BonusPhase = 0;
	}

	// shift bonus X coordinate
	BonusX -= BG_SPEED3;
}

// display bonus
void BonusDisp()
{
	int x = BonusX;
	if ((x >= -30*SCALE) && (BonusInx >= 0) && (x < WIDTH + 30*SCALE))
	{
		int inx = BonusInx;
		int y = BonusY;
		SCanvas* img = Bonus;
		
		// draw cloud
		DrawImageTrans(BonuscloudImg,			// image data and palettes
			BonusPhase * BonuscloudW, 0,		// source X, Y
			x - BonuscloudW/2, y - BonuscloudImg->h/2, // destination X, Y
			BonuscloudW, BonuscloudImg->h);		// width, height

		// draw bonus
		DrawImageTrans(img,					// image data and palettes
			0, 0,							// source X, Y
			x - img->w/2, y - img->h/2,		// destination X, Y
			img->w, img->h);				// width, height
	}
}

// collect bonus
void BonusCollect()
{
	int x = BonusX;
	if ((x >= -30*SCALE) && (BonusInx >= 0) && (x < WIDTH + 30*SCALE))
	{
		// check bonus collision
		if ((ActorX + ActorW/2 >= x) &&
			(ActorX - ActorW/2 < x) &&
			(ActorY + ActorH/2 >= BonusY) &&
			(ActorY - ActorH/2 < BonusY))
		{
			// add score
			if (BonusInx == BONUS_HEART)
				Life += 100;
			else
			{
				Score += BONUS_SCORE;
				AddPoint(BONUS_SCORE, BonusX, BonusY);
			}

			// sound
			if (SoundMode != SOUNDMODE_OFF)
				PlaySound(SOUNDCHAN_ACTORHIT, ZingSnd, False, SOUNDVOL_BONUS);

			if ((BonusInx >= BONUS_COLFIRST) && (BonusInx <= BONUS_COLLAST))
			{
				BonusMask |= BIT(BonusInx);
				if (BonusMask == BONUS_MASK_ALL)
				{
					// Picopad is complete
					Score += BONUS_BIGSCORE;
					DispHeader();
					RepaintCanvasDirty(TRUE);

					// delay 200 ms
					int k;
					for (k = 20; k > 0; k--)
					{
						Sleep(10);
						ShiftSound();
					}

					if (SoundMode != SOUNDMODE_OFF)
						PlaySound(SOUNDCHAN_ACTORHIT, BigbonusSnd, False, SOUNDVOL_BONUS);

					// delay 300 ms
					for (k = 30; k > 0; k--)
					{
						Sleep(10);
						ShiftSound();
					}

					BonusMask = 0;
					DispHeader();
					RepaintCanvasDirty(TRUE);
				}
			}

			// delete bonus
			BonusInx = -1;
		}
	}
}
