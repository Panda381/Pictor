
// ****************************************************************************
//
//                               Actor Missiles
//
// ****************************************************************************

#define MISSILE_SPEEDX_MIN	5		// missile speed in X direction - min
#define MISSILE_SPEEDX_MID	10		// missile speed in X direction - middle
#define MISSILE_SPEEDX_MAX	20		// missile speed in X direction - max

#define MISSILE_SPEEDY_MIN	2		// missile speed in Y direction - min
#define MISSILE_SPEEDY_MID	4		// missile speed in Y direction - middle
#define MISSILE_SPEEDY_MAX	10		// missile speed in Y direction - max

#define MISSILE_SPEED_FLASH0	10		// missile speed of flash of Stingray
#define MISSILE_SPEED_FLASH1	9		// *cos(22.5)
#define MISSILE_SPEED_FLASH2	7		// *cos(45)
#define MISSILE_SPEED_FLASH3	4		// *cos(67.5)

#define MISSILE_SPEED_BLACKHOLE	5		// black hole speed

#define MISSILE_LIFETIME_FLASH	(6*SCALE)	// flash lifetime
#define MISSILE_LIFETIME_SCREAM	(12*SCALE)	// scream lifetime

// actor missile images
extern SCanvas* ImgMissile[ACT_NUM];

// actor missile template
typedef struct {
	int			phasenum;	// number of animation phases
	int			reload;		// number of frames to reload weapon
	int			hits;		// number of hits
	Bool		bounce;		// bounce from edges
} sMissileTemp;

// actor missile templates
extern const sMissileTemp MissileTemp[ACT_NUM];

// actor missile descriptor
typedef struct {
	const sMissileTemp*	temp;		// missile template (NULL = not used)
	SCanvas*			img;		// missile image
	int					x, y;		// missile coordinate
	int					w, h;		// missile image size
	int					dx, dy;		// missile coordinate increment
	int					actinx;		// actor index
	int					phase;		// animation phase
	int					lifetime;	// lifetime counter
	int					hits;		// hit counter
	sEnemy*				target;		// target enemy (NULL = none)
} sMissile;

// actor missile list
#define MISSILE_MAX	30	// max. missiles
extern sMissile Missile[MISSILE_MAX];

// load missile images
BOOL LoadMissile();

// initialize actor missiles on new level
void InitMissile();

// add new missile
void AddMissile(const sMissileTemp* temp, SCanvas* img, int actinx, int x, int y, int dx, int dy, sEnemy* target);

// shift missiles
void MissileShift();

// display missiles
void MissileDisp();

// generate missile
void GenMissile();
