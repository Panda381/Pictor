
// ****************************************************************************
//
//                               Enemies
//
// ****************************************************************************

#define ENEMY_SPEED		3		// enemy speed
#define ENEMY_SLOWSPEED	1		// enemy slow speed
#define ENEMY_BORDERY	(10*SCALE)		// enemy reserve in Y direction

#define ENEMY_HIT_DIST	(20*SCALE)		// enemy hit distance
#define SUGAR_HIT_RADIUS (70*SCALE)		// sugar bomb radius

// move segment descriptor
typedef struct {
	int		steps;		// number of frame steps (0=endless, stop mark)
	int		dx;			// X increment
	int		dy;			// Y increment
	int		res;		// ... reserved (align)
} sMoveSeg;

// move segment macro (n=steps, 0=endless, stop mark)
#define MOVEL(n)	{ (n)*SCALE, -ENEMY_SPEED, 0 }					// left
#define MOVEL_SU(n)	{ (n)*SCALE, -ENEMY_SPEED, -ENEMY_SLOWSPEED }	// left, slow up
#define MOVEL_SD(n)	{ (n)*SCALE, -ENEMY_SPEED, ENEMY_SLOWSPEED }	// left, slow down
#define MOVELD(n)	{ (n)*SCALE, -ENEMY_SPEED, ENEMY_SPEED }		// left down
#define MOVED(n)	{ (n)*SCALE, 0, ENEMY_SPEED }					// down
#define MOVERD(n)	{ (n)*SCALE, ENEMY_SPEED, ENEMY_SPEED }			// right down
#define MOVER(n)	{ (n)*SCALE, ENEMY_SPEED, 0 }					// right
#define MOVERU(n)	{ (n)*SCALE, ENEMY_SPEED, -ENEMY_SPEED }		// right up
#define MOVEU(n)	{ (n)*SCALE, 0, -ENEMY_SPEED }					// up
#define MOVELU(n)	{ (n)*SCALE, -ENEMY_SPEED, -ENEMY_SPEED }		// left up

// move templates
extern const sMoveSeg* Moves[];

// enemy template descriptor
typedef struct {
	const char*	name;		// enemy name
	int		animmax;		// max. value of animation phase (before shift)
	int		animshift;		// shift of animation phase counter
	int		animmask;		// mask of animation phase after shift
} sEnemyTemp;

extern SCanvas* ImgEnemy[ENEMY_NUM];	// enemy images
extern const s8 EnemyScore[3];			// enemy score
extern const sEnemyTemp	EnemyTemp[ENEMY_NUM]; // enemy templates
extern sEnemyTemp	EnemyNow[ENEMY_LEVNUM]; // current enemy template
extern SCanvas* ImgEnemyNow[ENEMY_LEVNUM]; // current enemy images
extern int ImgEnemyWNow[ENEMY_LEVNUM];	// width of image phase

// enemy descriptor
typedef struct {
	const sEnemyTemp*	temp;		// enemy template, NULL = not used entry
	SCanvas*			img;		// enemy image
	int					w;			// width of phase image
	int					x, y;		// enemy coordinates - middle
	int					anim;		// animation phase
	const sMoveSeg*		move;		// pointer to move segments
	int					step;		// current step in move segment
	int					inx;		// enemy index (0..2)
} sEnemy;

extern sEnemy Enemy[ENEMY_MAX];		// enemies

// explosion descriptor
typedef struct {
	int		anim;		// animation phase (EXPLOSION_PHASES = not used)
	int		x, y;		// explosion coordinates - middle
} sExplosion;

#define EXPLOSIONS_MAX	20						// max. explosions
extern sExplosion Explosion[EXPLOSIONS_MAX];	// explosions
extern SCanvas* ImgExplosion;					// explosion image
#define EXPLOSION_PHASES	8					// number of phases

// load enemy images
BOOL LoadEnemy();

// initialize enemies on new level
void InitEnemy();

// add new enemy (temp = enemy template index 0..2)
void AddEnemy(const sEnemyTemp* temp, SCanvas* img, int w, int x, int y, const sMoveSeg* move, int inx);

// add explosion
void AddExplosion(int x, int y);

// generate new enemy
void NewEnemy();

// shift enemies
void EnemyShift();

// shift explosions
void ExplosionShift();

// display enemies
void EnemyDisp();

// display explosions
void ExplosionDisp();

// hit enemy by missile
void EnemyHit();
