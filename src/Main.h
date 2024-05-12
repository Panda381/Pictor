
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                               Main Module                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// global variables

// application startup
extern HINSTANCE	Instance;			// instance of the application

// screen size
extern int			ScreenWidth;		// screen client width
extern int			ScreenHeight;		// screen client height

extern HCURSOR		CurArrow;			// default application cursor (arrow)

extern BOOL KeyUp;		// key up is pressed
extern BOOL KeyDown;	// key down is pressed
extern BOOL KeyLeft;	// key left is pressed
extern BOOL KeyRight;	// key right is pressed
extern BOOL KeyShoot;	// key shoot is pressed (Ctrl, Enter)

// program state
enum {
	PROG_TITLE = 0,		// title screen
	PROG_START,			// game start screen
	PROG_GAME,			// game
	PROG_END,			// game end screen
	PROG_MENU,			// game menu
};

extern int ProgState;	// current program state
extern int MenuProgState; // previous program state, from game menu

// load one image (returns FALSE on error)
BOOL LoadImg(SCanvas** can, u16 id);

// check keys
inline BOOL KeyUpPressed() { return KeyUp; }
inline BOOL KeyDownPressed() { return KeyDown; }
inline BOOL KeyLeftPressed() { return KeyLeft; }
inline BOOL KeyRightPressed() { return KeyRight; }
inline BOOL KeyShootPressed() { return KeyShoot; }
