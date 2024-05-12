
// ****************************************************************************
//
//                               Game menu
//
// ****************************************************************************

#define MENU_COL_KEY	COL_YELLOW	// menu color - key
#define MENU_COL_MENU	COL_WHITE	// menu color - label

// top list entry
#define TOPNAME_LEN	8	// top name length
typedef struct {
	s32		score;				// score
	char	name[TOPNAME_LEN];	// name with spaces
} sTop;

#define TOP_NUM		8	// number of top entries
extern sTop Top[TOP_NUM];	// top list

// load top list (returns max. score)
s32 LoadTop();

// save top list (returns FALSE on error)
BOOL SaveTop();

// display top score
void DispTop();

// display game menu
void DispGameMenu();

// control key left
void GameEndLeft();

// control key right
void GameEndRight();

// open game menu
void GameMenu();
