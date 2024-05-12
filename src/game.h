
// ****************************************************************************
//
//                                Game
//
// ****************************************************************************

extern int Level;			// current scene level - 1
extern int SubFrame;		// sub-frame counter
extern int CurrentFrame;	// current game frame
extern int MaxFrame;		// max. game frames

// start first level
void StartGame0();

// initialize new game (level = 0..)
void NewGame(int level);

// display game screen
void DispGame();

// shift game
void ShiftGame();
