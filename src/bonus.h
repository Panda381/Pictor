
// ****************************************************************************
//
//                                 Bonus
//
// ****************************************************************************

#define BONUS_SCORE			200		// bonus points
#define BONUS_BIGSCORE		10000	// big bonus points
#define BONUS_BIGSCORE_TEXT	"10000"	// big bonus text
#define BONUS_BIGSCORE_LEN	5		// length of bit bonus text

// bonus index
#define BONUS_HEART	0		// heart
#define BONUS_TOP	1		// Picopad top cover
#define BONUS_CPU	2		// Picopad CPU
#define BONUS_DISP	3		// Picopad display
#define BONUS_BAT	4		// Picopad battery
#define BONUS_ON	5		// Picopad is ON

#define BONUS_NUM	6		// number of bonuses

#define BONUS_COLFIRST	1		// index of first collected bonus
#define BONUS_COLLAST	4		// index of last collected bonus
#define BONUS_PHASES	6		// number of animation phases

// bonus mask
#define BONUS_MASK_TOP	BIT(BONUS_TOP)	// Picopad top cover
#define BONUS_MASK_CPU	BIT(BONUS_CPU)	// Picopad CPU
#define BONUS_MASK_DISP	BIT(BONUS_DISP)	// Picopad display
#define BONUS_MASK_BAT	BIT(BONUS_BAT)	// Picopad battery

#define BONUS_MASK_ALL	(B1+B2+B3+B4)	// Picopad all components

// bonus images
extern SCanvas* ImgBonus[BONUS_NUM];

// current bonus
extern int BonusMask;			// mask of collected Picopad components (BONUS_MASK_*)
extern int BonusInx;			// index of current bonus (-1 if bonus is not active)
extern SCanvas* Bonus;			// current bonus image
extern int BonusX;				// coordinate X of current bonus
extern int BonusY;				// coordinate Y of current bonus
extern int BonusPhase;			// bonus animation phase (0..2)
extern SCanvas* BonuscloudImg;	// bonus cloud image
extern int BonuscloudW;			// width of bonus cloud image

// load bonus images
BOOL LoadBonus();

// initialize bonus on start of next level (requires initialized EnemyRandSeed)
void InitBonus();

// shift bonus
void BonusShift();

// display bonus
void BonusDisp();

// collect bonus
void BonusCollect();
