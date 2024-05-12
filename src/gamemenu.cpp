
// ****************************************************************************
//
//                               Game menu
//
// ****************************************************************************

#include "Include.h"

sTop Top[TOP_NUM];	// top list

// load top list (returns max. score)
s32 LoadTop()
{
	// reset top list
	int i;
	memset(Top, ' ', sizeof(Top));
	for (i = 0; i < TOP_NUM; i++) Top[i].score = 0;

	// load top file
	FILE* f;
	f = fopen(TOPLIST_NAME, "rb");
	if (f != NULL)
	{
		fread(Top, 1, sizeof(Top), f);
		fclose(f);
	}

	// get max. score
	return Top[0].score;
}

// save top list (returns FALSE on error)
BOOL SaveTop()
{
	FILE* f;
	f = fopen(TOPLIST_NAME, "wb");
	if (f == NULL) return FALSE;
	size_t k = fwrite(Top, 1, sizeof(Top), f);
	fclose(f);
	return (k == sizeof(Top));
}

// display top score
void DispTop()
{
#define TOPBUFN 50
	char buf[TOPBUFN];
	char namebuf[TOPNAME_LEN+1];

	int y = 95*SCALE;

	// display top list
	DrawText("Top List", (WIDTH - 8*8*SCALE)/2, y, COL_GREEN);
	y += 20*SCALE;

	int i; // y=240-8*15 = 120
	for (i = 0; i < TOP_NUM; i++)
	{
		memcpy(namebuf, Top[i].name, TOPNAME_LEN);
		namebuf[TOPNAME_LEN] = 0;
		sprintf(buf, "%d: %07d %s", i+1, Top[i].score, namebuf);
		DrawText(buf, 8*SCALE, y, COL_WHITE);
		y += 15*SCALE;
	}
}

// display game menu
void DispGameMenu()
{
	// clear screen
	DrawClear();

	// version
	DrawTextSmall(VERSION, (WIDTH - (int)strlen(VERSION)*8)/2, 2, COL_LTGRAY);

	// display title
	int y = 10*SCALE;
	DrawText("Game Menu", (WIDTH - 9*8*SCALE)/2, y, COL_GREEN);
	y += 20*SCALE;

#define MENUX	(8*SCALE)

	// display menu
	DrawText("Esc", MENUX, y, MENU_COL_KEY);
	DrawText("..... Continue", MENUX+4*8*SCALE, y, MENU_COL_MENU);
	y += 15*SCALE;

	DrawText("Enter", MENUX, y, MENU_COL_KEY);
	DrawText("... Exit", MENUX+6*8*SCALE, y, MENU_COL_MENU);
	y += 15*SCALE;

	DrawText("Space", MENUX, y, MENU_COL_KEY);
	DrawText("... Sound is", MENUX+6*8*SCALE, y, MENU_COL_MENU);
	DrawText((SoundMode == SOUNDMODE_OFF) ? "off" :
				((SoundMode == SOUNDMODE_SOUND) ? "ON" : "ALL"),
				MENUX+19*8*SCALE, y, MENU_COL_MENU);
//	y += (15+15)*SCALE;

	// display top list
	DispTop();
}

// open game menu
void GameMenu()
{
	// stop sounds
	StopAllSound();

	// save old state
	MenuProgState = ProgState;

	// set new program state
	ProgState = PROG_MENU;

	// repaint screen
	RepaintAll(TRUE);
}
