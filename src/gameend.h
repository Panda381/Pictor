
// ****************************************************************************
//
//                               Game end
//
// ****************************************************************************

extern int GameEndDelay;	// delay counter of game end screen
extern BOOL GameEndError;	// error of game end
extern char PlayerName[TOPNAME_LEN+1];
extern int PlayerNameInx;

// display game end
void DispGameEnd();

// shift game end animation
void ShiftGameEnd();

// open game end
void GameEnd();

// input character
void GameEndChar(char ch);

// control key
void KeyGameEnd(int key);

// close game end (Enter)
void CloseGameEnd();
