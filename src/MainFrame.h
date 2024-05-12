
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                              Application Window                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

extern HWND		MainFrame;				// main frame
extern LPCTSTR	MainFrameClass;			// name of the mainframe class
extern LPCTSTR	MainFrameTitle;			// title
extern RECT		MainFrameRect;			// main frame absolute rect
extern RECT		MainFrameClientRect;	// main frame client rect

extern int		MainFrameLeft;			// main frame X coordinate
extern int		MainFrameTop;			// main frame Y coordinate
extern int		MainFrameWidth;			// main frame width
extern int		MainFrameHeight;		// main frame height

extern int		MainFrameClientLeft;	// inner X coordinate of main frame
extern int		MainFrameClientTop;		// inner Y coordinate of main frame
extern int		MainFrameClientWidth;	// inner width of main frame (0=auto)
extern int		MainFrameClientHeight;	// inner height of main frame (0=auto)

extern BOOL CanDirty; // canvas needs to repaint

// timer
#define TIMERID		5678

#define CAN_W	(320*3) //KEY_W	// drawing canvas width (=960)
#define CAN_H	(240*3) //((DISP_H+KEY_H) // drawing canvas height (=720)

#define MAINFRAMECLIENTWIDTH CAN_W			// main frame client width (=960)
#define MAINFRAMECLIENTHEIGHT CAN_H			// main frame client height (=720)

// display position
//extern u8 PosY;		// row

///////////////////////////////////////////////////////////////////////////////
// update main frame size and position

void MainFrameUpdateRect();

///////////////////////////////////////////////////////////////////////////////
// write main frame position ini

void MainFrameWriteIni();

/////////////////////////////////////////////////////////////////////////////
// create main frame (returns TRUE if success)

BOOL MainFrameCreate();

/////////////////////////////////////////////////////////////////////////////
// repaint screen

void RepaintAll(BOOL update);

/////////////////////////////////////////////////////////////////////////////
// repaint drawing canvas

void RepaintCanvas();

/////////////////////////////////////////////////////////////////////////////
// repaint canvas if dirty

void RepaintCanvasDirty(BOOL update);
