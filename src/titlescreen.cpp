
// ****************************************************************************
//
//                               Title screen
//
// ****************************************************************************

#include "Include.h"

SCanvas* ImgTitle;	// title screen

// display title screen
void DispTitleScreen()
{
#define TOPBUFN 30
	char buf[TOPBUFN];

	// draw title screen
	DrawImage(ImgTitle, 0, 0, 0, 0, WIDTH, HEIGHT);

#if DEBUG		// 1=debug mode
	DrawText(">DEBUG!<", 17*SCALE, 27*SCALE, COL_RED);
#endif

	// draw max. score
	sprintf(buf, "Max Score: %06d", MaxScore);
	DrawTextSmall(buf, 80, 240*SCALE-30, COL_GREEN);
}

// title screen
void TitleScreen()
{
	// set program state
	ProgState = PROG_TITLE;

	// repaint screen
	RepaintAll(TRUE);
}
