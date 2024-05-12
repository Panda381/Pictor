
// ****************************************************************************
//
//                                 Enemy bullets
//
// ****************************************************************************

#define BULLET_SPEED	5		// bullet speed

// enemy bullet images
#define BULLET_TEMP_NUM	3
extern SCanvas* ImgBullet[BULLET_TEMP_NUM];

// enemy bullet descriptor
typedef struct {
	SCanvas*	img;		// bullet image (NULL = not used)
	int			x, y;		// bullet coordinate
} sBullet;

// enemy bullet list
#define BULLET_MAX	30	// max. bullets
extern sBullet Bullet[BULLET_MAX];

// load bullet images
BOOL LoadBullet();

// initialize enemy bullets on new level
void InitBullet();

// add new bullet
void AddBullet(SCanvas* img, int x, int y);

// shift bullets
void BulletShift();

// display bullets
void BulletDisp();
