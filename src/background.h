
// ****************************************************************************
//
//                               Background
//
// ****************************************************************************

extern int BackInx;		// index of background
extern int BackPhase1, BackPhase2, BackPhase3; // background phases

// background images
extern SCanvas* ImgBg1[BG_NUM];
extern SCanvas* ImgBg2[BG_NUM];
extern SCanvas* ImgBg3[BG_NUM];

// background name
extern const char* BackName[BG_NUM];

// background speed
#define BG_SPEED1	1	// layer 1 speed
#define BG_SPEED2	2	// layer 2 speed
#define BG_SPEED3	3	// layer 3 speed

#define BG_Y_MIN	TIT_HEIGHT	// background minimal Y coordinate

// background layer 1 (Y = 60..539)
#define BG_Y		BG_Y_MIN	// background Y coordinate (= 60)
#define BG_WIDTH	(640*SCALE)	// width of background image (= 1920)
#define BG_HEIGHT	(160*SCALE)		// height of background image (= 480)

// - Overlap layer 1 and layer 2 by 3*70=210 lines
// - Layer 2 must add 3*10=30 lines

// background layer 2 (Y = 450..569)
#define BG2_Y		(BG_Y+BG_HEIGHT-70*SCALE) // background Y coordinate (= 330)
#define BG2_WIDTH	(640*SCALE) 	// width of background image (= 1920)
#define BG2_HEIGHT	(80*SCALE)		// max. height of background image (= 240)

// - Overlap layer 2 and layer 3 by 3*10=30 lines
// - Layer 3 must add 3*30=90 lines

// background layer 3 (Y = 540..659)
#define BG3_Y		(BG2_Y+BG2_HEIGHT-10*SCALE) // background Y coordinate (= 540)
#define BG3_WIDTH	(640*SCALE)	// width of background image (= 1920)
#define BG3_HEIGHT	(40*SCALE)	// height of background image (= 120)

// load backgrounds on program start
BOOL LoadBack();

// initialize backgrounds on new level
void InitBack();

// display backgrounds
void DispBack();

// shift backgrounds
void ShiftBack();
