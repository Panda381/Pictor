
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                               Main Module                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "Include.h"

///////////////////////////////////////////////////////////////////////////////
// global variables

// application startup
HINSTANCE	Instance;					// instance of the application

// screen size
int			ScreenWidth;				// screen client width
int			ScreenHeight;				// screen client height

HCURSOR		CurArrow = NULL;			// default application cursor (arrow)

BOOL KeyUp = FALSE;		// key up is pressed
BOOL KeyDown = FALSE;	// key down is pressed
BOOL KeyLeft = FALSE;	// key left is pressed
BOOL KeyRight = FALSE;	// key right is pressed
BOOL KeyShoot = FALSE;	// key shoot is pressed (Ctrl)

int ProgState;			// current program state
int MenuProgState;		// previous program state, from game menu

///////////////////////////////////////////////////////////////////////////////
// load one image (returns FALSE on error)

BOOL LoadImg(SCanvas** can, u16 id)
{
	// find resource
	HRSRC res = ::FindResource(Instance, MAKEINTRESOURCE(id), "TGA");
	if (res == NULL) return FALSE;

	// lock resource
	HGLOBAL glob = ::LoadResource(Instance, res);
	if (glob == NULL) return FALSE;

	// get image address
	u8* a = (u8*)::LockResource(glob);
	if (a == NULL) return FALSE;

	// load image dimension
	TGAHEADER* t = (TGAHEADER*)a;
	if (t->DataTypeCode != 2) return FALSE; // uncompressed RGB
	int w = t->Width;
	int h = t->Height;
	int c = t->BitsPerPixel/8;

	// load image data
	*can = NewCanvas(w, h, c);
	if (*can == NULL) return FALSE;
	memcpy((*can)->d, a + TGAHEADSIZE, w*h*c);

	// vertical flip
	VFlipCanvas(*can);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// load all images

BOOL LoadAllImg()
{
	// create drawing canvas
	Canvas = NewCanvas(CAN_W, CAN_H, 3);
	if (Canvas == NULL) return FALSE;

	// load font
	if (!LoadImg(&Font, TGA_FONT)) return FALSE;
	if (!LoadImg(&FontSmall, TGA_FONTSMALL)) return FALSE;

	// load title screem
	if (!LoadImg(&ImgTitle, TGA_TITLE)) return FALSE;

	// load backgrounds on program start
	if (!LoadBack()) return FALSE;

	// load actor images
	if (!LoadAct()) return FALSE;

	// load enemy images
	if (!LoadEnemy()) return FALSE;

	// load missile images
	if (!LoadMissile()) return FALSE;

	// load bullet images
	if (!LoadBullet()) return FALSE;

	// load bonus images
	if (!LoadBonus()) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// translate key codes (returns TRUE to process message)

BOOL TransKey(MSG* msg)
{
	if ((msg->message == WM_KEYDOWN) || (msg->message == WM_SYSKEYDOWN))
	{
		int key = (int)msg->wParam;

		switch (key)
		{
		// move up
		case VK_UP:
			KeyUp = TRUE;
			return FALSE;

		// move down
		case VK_DOWN:
			KeyDown = TRUE;
			return FALSE;

		// move left
		case VK_LEFT:
			KeyLeft = TRUE;
			if (ProgState == PROG_END) GameEndLeft();
			return FALSE;

		// move right
		case VK_RIGHT:
			KeyRight = TRUE;
			if (ProgState == PROG_END) GameEndRight();
			return FALSE;

#if DEB_CUSTOMLEVEL					// 1=select level using Home/End keys
		case VK_HOME:
			Level++;
			NewGame(Level);
			return FALSE;
#endif

#if DEB_CUSTOMLEVEL					// 1=select level using Home/End keys
		case VK_END:
			Level--;
			if (Level < 0) Level = 0;
			NewGame(Level);
			return FALSE;
#endif

		// next actor
		case VK_NEXT:
			if (ProgState == PROG_GAME)
			{
				SetActor(ActInx+1);
			}
			else if (ProgState == PROG_START)
			{
				SetActor(ActInx+1);
				ActorReload = 0;
				SubFrame = 0;
				ShiftGameStart();
				RepaintAll(TRUE);
			}
			return FALSE;

		// previous actor
		case VK_PRIOR:
			if (ProgState == PROG_GAME)
			{
				SetActor(ActInx-1);
			}
			else if (ProgState == PROG_START)
			{
				SetActor(ActInx-1);
				ActorReload = 0;
				SubFrame = 0;
				ShiftGameStart();
				RepaintAll(TRUE);
			}
			return FALSE;

		// shooting
		case VK_CONTROL:
			KeyShoot = TRUE;
			return FALSE;

		// Enter
		case VK_RETURN:
			switch (ProgState)
			{
			case PROG_TITLE:
				StartGame0();
				break;

			case PROG_START:
				MusicStart();
				ProgState = PROG_GAME;
				RepaintAll(TRUE);
				break;

			case PROG_END:
				CloseGameEnd();
				break;

			case PROG_MENU:
				if (MenuProgState == PROG_TITLE)
				{
					::KillTimer(MainFrame, TIMERID);
					::PostQuitMessage(0);
				}
				else
					TitleScreen();
				break;
			}
			return FALSE;

		// Esc
		case VK_ESCAPE:
			switch (ProgState)
			{
			// title screen
			case PROG_TITLE:
			// game start
			case PROG_START:
			// game
			case PROG_GAME:
				GameMenu();
				break;

			// game end
			case PROG_END:
				TitleScreen();
				break;

			// menu
			case PROG_MENU:
				ProgState = MenuProgState;
				RepaintAll(TRUE);
				if (ProgState == PROG_GAME) MusicStart();
				break;
			}
			return FALSE;

		// Space
		case VK_SPACE:
			if (ProgState == PROG_MENU)
			{
				SoundMode++;
				if (SoundMode > 2) SoundMode = 0;
				RepaintAll(TRUE);
				return FALSE;
			}
			else if (ProgState == PROG_END)
			{
				GameEndChar(32);
				return FALSE;
			}
			return TRUE;
		}
	}

	if (msg->message == WM_KEYUP)
	{
		int key = (int)msg->wParam;

		switch (key)
		{
		// move up
		case VK_UP:
			KeyUp = FALSE;
			return FALSE;

		// move down
		case VK_DOWN:
			KeyDown = FALSE;
			return FALSE;

		// move left
		case VK_LEFT:
			KeyLeft = FALSE;
			return FALSE;

		// move right
		case VK_RIGHT:
			KeyRight = FALSE;
			return FALSE;

		// shooting
		case VK_CONTROL:
			KeyShoot = FALSE;
			return FALSE;
		}
	}

	// input character - enter player's name
	if (msg->message == WM_CHAR)
	{
		char ch = (char)msg->wParam;
		if (ProgState == PROG_END) GameEndChar(ch);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// program entry point

int APIENTRY _tWinMain(HINSTANCE inst, HINSTANCE prev, LPTSTR cmd, int show)
{
	// save instance handle
	Instance = inst;

	// get screen size
	ScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	// initialize random
	RandSeed = time(NULL) + GetCurrentProcessId();
	RandU32(); RandU32(); RandU32(); RandU32();

	// load default application cursor
	CurArrow = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));

	// load all images
	if (!LoadAllImg()) return 254;

	// load all sounds
	if (!LoadAllSnd()) return 253;

	// create main frame
	if (!MainFrameCreate()) return 252;

	// load top list and max. score
	MaxScore = LoadTop();

	// start title screen
	TitleScreen();

	// message loop
	MSG msg;
	for (;;)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			if (TransKey(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
	return 0;
}
