
// ****************************************************************************
//
//                               Game end
//
// ****************************************************************************

#include "Include.h"

int GameEndDelay;	// delay counter of game end screen
BOOL GameEndError;	// error of game end
char PlayerName[TOPNAME_LEN+1] = "        ";
int PlayerNameInx = 0;

// display game end
void DispGameEnd()
{
#define TOPBUFN 30
	char buf[TOPBUFN];

	// clear screen
	DrawClear();

	// display title
	DrawText("Game Over", (WIDTH - 9*8*SCALE)/2, 0, COL_RED);
	int y = 20*SCALE;

	// error
	if (GameEndError)
	{
			y += 20*SCALE;
			DrawText("Error writing top list!", (WIDTH - 23*8*SCALE)/2, y, COL_RED);
			y += 20*SCALE;
			DrawText("Press Esc to continue...", (WIDTH - 24*8*SCALE)/2, y, COL_RED);
			DispTop();
			return;
	}

	// high score
	if (Score > MaxScore)
	{
		sprintf(buf, "New MaxScore %07d", Score);
		DrawText(buf, (WIDTH - 20*8*SCALE)/2, y, COL_MAGENTA);

		if (GameEndDelay <= 0)
		{
			y += 16*SCALE;
			DrawText("Enter your name and press Enter", (WIDTH - 31*8*SCALE)/2, y, COL_YELLOW);
			y += 15*SCALE;
			DrawText("or press Esc to not save the score", (WIDTH - 34*8*SCALE)/2, y, COL_YELLOW);
			y += 16*SCALE;
			DrawText(PlayerName, (WIDTH - 8*8*SCALE)/2, y, COL_AZURE);
			if (((GameEndDelay >> 1) & B0) == 0) DrawText("_", (WIDTH - 8*8*SCALE)/2 + PlayerNameInx*8*SCALE, y + 4*SCALE, COL_AZURE);
		}
	}
	else
	{
		sprintf(buf, "Your Score %07d", Score);
		DrawText(buf, (WIDTH - 18*8*SCALE)/2, y, COL_MAGENTA);
		
		if (GameEndDelay <= 0)
		{
			y += 16*SCALE;
			DrawText("Press Esc to continue...", (WIDTH - 24*8*SCALE)/2, y, COL_YELLOW);
		}
	}

	// display top list
	DispTop();
}

// shift game end animation
void ShiftGameEnd()
{
	// shift frame
	SubFrame++;
	if (SubFrame >= SUBFRAMES)
	{
		SubFrame = 0;

		// shift delay counter
		GameEndDelay--;
	}
}

// open game end
void GameEnd()
{
	// stop all sounds
	StopAllSound();

	// set new program state
	ProgState = PROG_END;
	GameEndError = FALSE;

	// get name of best player
	memcpy(PlayerName, Top[0].name, TOPNAME_LEN);
	PlayerNameInx = 0;

	// reset frame counter
	SubFrame = 0;
	GameEndDelay = 6; // delay 1/2 second

	// repaint screen
	RepaintAll(TRUE);
}

// input character
void GameEndChar(char ch)
{
	if (GameEndDelay > 0) return;

	// valid character
	if ((ch >= 32) && (ch <= 127))
	{
		PlayerName[PlayerNameInx] = ch;
		GameEndRight();
	}

	// BS
	if (ch == 8)
	{
		GameEndLeft();
	}
}

// control key left
void GameEndLeft()
{
	if (GameEndDelay > 0) return;
	if (PlayerNameInx > 0) PlayerNameInx--;
	RepaintAll(TRUE);
}

// control key right
void GameEndRight()
{
	if (GameEndDelay > 0) return;
	if (PlayerNameInx < TOPNAME_LEN-1) PlayerNameInx++;
	RepaintAll(TRUE);
}

// close game end (Enter)
void CloseGameEnd()
{
	if (GameEndDelay > 0) return;
	if ((Score > MaxScore) && !GameEndError)
	{
		// move top table
		memmove(&Top[1], &Top[0], (TOP_NUM-1)*sizeof(sTop));

		// insert new score
		Top[0].score = Score;
		MaxScore = Score;
		memcpy(Top[0].name, PlayerName, TOPNAME_LEN);

		// save table
		if (!SaveTop())
		{
			// error flag
			GameEndError = TRUE;
			RepaintAll(TRUE);
			return;
		}
	}
	TitleScreen();
}
