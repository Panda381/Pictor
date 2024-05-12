
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                              Application Window                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "Include.h"

///////////////////////////////////////////////////////////////////////////////
// variables

HWND		MainFrame = NULL;			// main frame
LPCTSTR		MainFrameClass = _T("Pictor_MainFrame"); // name of the mainframe class
LPCTSTR		MainFrameTitle = _T("Pictor"); // title
RECT		MainFrameRect;				// main frame absolute rect
RECT		MainFrameClientRect;		// main frame client rect

int			MainFrameLeft;				// main frame X coordinate
int			MainFrameTop;				// main frame Y coordinate
int			MainFrameWidth;				// main frame width
int			MainFrameHeight;			// main frame height

int			MainFrameClientLeft;		// inner X coordinate of main frame
int			MainFrameClientTop;			// inner Y coordinate of main frame
int			MainFrameClientWidth;		// inner width of main frame (0=auto)
int			MainFrameClientHeight;		// inner height of main frame (0=auto)

BOOL CanDirty = TRUE; // canvas needs to repaint

// main frame style
#define MAINFRAME_STYLE	(WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_MINIMIZEBOX | \
				WS_POPUP | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)
#define MAINFRAME_STYLEEX	(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE)

///////////////////////////////////////////////////////////////////////////////
// update main frame size and position

void MainFrameUpdateRect()
{
	// get main frame absolute rect
	::GetWindowRect(MainFrame, &MainFrameRect);
	MainFrameLeft = MainFrameRect.left;
	MainFrameTop = MainFrameRect.top;
	MainFrameWidth = MainFrameRect.right - MainFrameRect.left;
	MainFrameHeight = MainFrameRect.bottom - MainFrameRect.top;

	// get client rectangle
	::GetClientRect(MainFrame, &MainFrameClientRect);
	MainFrameClientLeft = MainFrameClientRect.left;
	MainFrameClientTop = MainFrameClientRect.top;
	MainFrameClientWidth = MainFrameClientRect.right - MainFrameClientRect.left;
	MainFrameClientHeight = MainFrameClientRect.bottom - MainFrameClientRect.top;
}

/////////////////////////////////////////////////////////////////////////////
// repaint screen

void RepaintAll(BOOL update)
{
	switch (ProgState)
	{
	// title screen
	case PROG_TITLE:
		DispTitleScreen();
		break;

	// game start screen
	case PROG_START:
		DispGameStart();
		break;

	// game screen
	case PROG_GAME:
		DispGame();
		break;

	// game end screen
	case PROG_END:
		DispGameEnd();
		break;

	// game menu
	case PROG_MENU:
		DispGameMenu();
		break;
	}

	// repaint canvas to the screen
	RepaintCanvasDirty(update);
}

/////////////////////////////////////////////////////////////////////////////
// message service of main frame

LRESULT CALLBACK MainFrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
//	int wmId, wmEvent;

	// window messages
	switch (msg)
	{
	// close application
	case WM_CLOSE:
		::DestroyWindow(MainFrame);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			RepaintCanvasDirty(FALSE);
		}
		break;

	case WM_DESTROY:
		::KillTimer(MainFrame, TIMERID);
		::PostQuitMessage(0);
		break;

	case WM_MOVE:
		MainFrameUpdateRect();
		break;

	case WM_TIMER:
		if (ProgState == PROG_GAME)
		{
			RepaintAll(TRUE);
			ShiftGame();
		}
		else if (ProgState == PROG_START)
		{
			ShiftGameStart();
			if (SubFrame == 0) RepaintAll(TRUE);
		}
		else if (ProgState == PROG_END)
		{
			ShiftGameEnd();
			if (SubFrame == 0) RepaintAll(TRUE);
		}
		RepaintCanvasDirty(TRUE);

		// shift sound channels
		ShiftSound();

		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// create main frame (returns TRUE if success)

BOOL MainFrameCreate()
{
	// register window class
	WNDCLASS wcl;
	wcl.style = CS_OWNDC;
	wcl.lpfnWndProc = MainFrameProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = Instance;
	wcl.hIcon = ::LoadIcon(Instance, MAKEINTRESOURCE(IDI_ICON));
	wcl.hCursor = CurArrow;
	wcl.hbrBackground = NULL;
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = MainFrameClass;
	::RegisterClass(&wcl);

	// adjust window size
	MainFrameClientRect.left = 0;
	MainFrameClientRect.top = 0;
	MainFrameClientRect.right = MAINFRAMECLIENTWIDTH;
	MainFrameClientRect.bottom = MAINFRAMECLIENTHEIGHT;
	::AdjustWindowRectEx(&MainFrameClientRect, MAINFRAME_STYLE, NULL, MAINFRAME_STYLEEX);
	MainFrameWidth = MainFrameClientRect.right - MainFrameClientRect.left;
	MainFrameHeight = MainFrameClientRect.bottom - MainFrameClientRect.top;

	// read frame position
	MainFrameLeft = (ScreenWidth - MainFrameWidth)/2;
	MainFrameTop = (ScreenHeight - MainFrameHeight - 10)/2;
	if (MainFrameTop < 0) MainFrameTop = 0;
	if (MainFrameTop > ScreenHeight - 50) MainFrameTop = ScreenHeight - 50;
	if (MainFrameLeft < 30 - MainFrameWidth) MainFrameLeft = 30 - MainFrameWidth;
	if (MainFrameLeft > ScreenWidth - 30) MainFrameLeft = ScreenWidth - 30;

	// create window
	MainFrame = ::CreateWindowEx(
		MAINFRAME_STYLEEX,	// style ex
		MainFrameClass,		// class name
		MainFrameTitle,		// title
		MAINFRAME_STYLE,	// style
		MainFrameLeft,		// x
		MainFrameTop,		// y
		MainFrameWidth,		// width
		MainFrameHeight,	// height
		NULL,				// parent
		NULL,				// menu
		Instance,			// instance
		NULL);				// lpParam

	MainFrameUpdateRect();

	// create timer
	::SetTimer(MainFrame, TIMERID, TIMERTIME, NULL);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// repaint drawing canvas

void RepaintCanvas()
{
	// get client window DC
	HDC dc = ::GetDC(MainFrame);

	int w = Canvas->w;
	int h = Canvas->h;

	// prepare bitmap info header
	BITMAPINFO bmp;
	memset(&bmp, 0, sizeof(BITMAPINFO));
	bmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmp.bmiHeader.biHeight = -h;
	bmp.bmiHeader.biWidth = w;
	bmp.bmiHeader.biPlanes = 1;
	bmp.bmiHeader.biBitCount = 24;
	bmp.bmiHeader.biCompression = BI_RGB;

	// stretch mode - no smoothing
	::SetStretchBltMode(dc, COLORONCOLOR);

	::StretchDIBits(dc,
		0, 0, w, h,
		0, 0, w, h,
		Canvas->d, &bmp, DIB_RGB_COLORS, SRCCOPY);

	// release DC
	::ReleaseDC(MainFrame, dc);
}

/////////////////////////////////////////////////////////////////////////////
// repaint canvas if dirty

void RepaintCanvasDirty(BOOL update)
{
	if (CanDirty)
	{
		CanDirty = FALSE;
		RepaintCanvas();

		if (update)
		{
			::InvalidateRect(MainFrame, NULL, TRUE);
			::UpdateWindow(MainFrame);
		}
	}
}
