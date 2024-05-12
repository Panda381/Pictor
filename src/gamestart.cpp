
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "Include.h"

int GameStartDelay;	// delay counter of game start screen

// shift game start animation
void ShiftGameStart()
{
	if (SubFrame == 0)
	{
		// start position, shift animation
		ActorX = ActorMinX;
		if (Actor.walk)
		{
			// Jill
			if (ActInx == ACT_JILL)
			{
				// Y of walking actor
				ActorY = ActorMaxY + 6*SCALE;

				// set phase of standing actor animation
				ActorPhase = JILL_STAND;
			}

			// other walking actors
			else
			{
				// Y of walking actor
				ActorY = ActorMaxY;

				// set phase of standing actor animation
				ActorPhase = 0;
			}
		}
		else
		{
			ActorY = (ActorMinY + ActorMaxY)/2; // Y of flying actor

			// shift flying actor animation
			ShiftActor();
		}
	}

	// shift frame
	SubFrame++;
	if (SubFrame >= SUBFRAMES)
	{
		SubFrame = 0;

		// shift delay counter
		if (GameStartDelay > 0) GameStartDelay--;
	}
}

// display game start
void DispGameStart()
{
	// display background
	DispBack();

	// display header
	DispHeader();

	// display foot
	DispFoot();

	// shadow screen
	DispShadow();

	// display actor
	DispActor();

#define OPEN_X	(190*SCALE)
	int y = (Level < BG_NUM) ? 20*SCALE : 60*SCALE;

	// Title
	int len = sprintf(DecNumBuf, "%d", Level + 1);
	len += 16; // add text "Entering Sector "
	int x = OPEN_X - len*8*SCALE/2;
	DrawText("Entering Sector", x, y, COL_WHITE);
	x += 16*8*SCALE;
	DrawText(DecNumBuf, x, y, COL_WHITE);
	y += 16*SCALE;

	len = (int)strlen(BackName[BackInx]);
	DrawText(BackName[BackInx], OPEN_X - len*8*SCALE/2, y, COL_AZURE);

	// new helper
	if (Level < BG_NUM)
	{
		y += 16*SCALE; // 20 + 16 + 16 = 52
		const sActorTemp* a = &ActorTemp[BackInx+1];
		SCanvas* img = ImgAct[BackInx+1];
		// y_help = 190, height_of_area = 190 - 52 = 138, height_of_actor = 16 + 16 + a->h + 16
		y += (138*SCALE - ((16 + 16 + 16)*SCALE + img->h))/2;

		// Helper
		DrawText("New Helper", OPEN_X - 10*8*SCALE/2, y, COL_WHITE);
		y += 16*SCALE;
		len = (int)strlen(a->name);
		DrawText(a->name, OPEN_X - len*8*SCALE/2, y, COL_GREEN);
		y += 16*SCALE;

		// draw helper
		int w = img->w/4;
		DrawImageTrans(img,		// source image data
				0,				// source X
				0,				// source Y
				OPEN_X - w/2,	// destination X
				y,				// destination Y
				w,				// image width
				img->h);		// image height
		y += img->h;

		// Attack
		len = (int)strlen(a->missile) + 7;
		x = OPEN_X  - len*8*SCALE/2;
		DrawText("Attack:", x, y, COL_RED);
		DrawText(a->missile, x+7*8*SCALE, y, COL_RED);
	}

	// help
	if (GameStartDelay <= 0)
	{
		DrawText("Press PgUp/PgDn to transform", OPEN_X - 28*8*SCALE/2, 190*SCALE, COL_YELLOW);
		DrawText("Press Enter to start game", OPEN_X - 25*8*SCALE/2, 205*SCALE, COL_YELLOW);
	}
}

// open game start
void GameStart()
{
	// set new program state
	ProgState = PROG_START;

	// reset frame counter
	SubFrame = 0;
	GameStartDelay = 6; // delay 1/2 second

	// initialize coordinates
	ShiftGameStart();

	// repaint screen
	RepaintAll(TRUE);
}
