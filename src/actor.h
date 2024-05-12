
// ****************************************************************************
//
//                                  Actors
//
// ****************************************************************************

// actor indices
#define ACT_JILL		0	// Jill's index
#define ACT_BIRD		1	// Bird's index
#define ACT_PARROT		2	// Parrot's index
#define ACT_SCARABEUS	3	// Scarabeus index
#define ACT_STINGRAY	4	// Stingray index
#define ACT_ICEBIRD		5	// Ice Bird's index
#define ACT_DRAGON		6	// Dragon's index
#define ACT_GHOST		7	// Ghost's index
#define ACT_R2D2		8	// R2D2's index
#define ACT_UFO			9	// UFO index
#define ACT_SQUIRREL	10	// Squirrel's index
#define ACT_GINGER		11	// Gingerbread Man's index
#define ACT_DEVILBIRD	12	// Devil Bird's index

#define ACT_MINX		(3*SCALE)	// actor's left minimal X coordinate
#define ACT_MAXX		(WIDTH-20*SCALE) // actor's right maximal X coordinate
#define ACT_MINY		(TIT_HEIGHT+3*SCALE) // actor's top minimal Y coordinate
#define ACT_MAXY		(HEIGHT-FOOT_HEIGHT-3*SCALE) // actor's bottom maximal Y coordinate
#define ACT_SPEEDX		5			// actor's speed in X direction
#define ACT_SPEEDY		5			// actor's speed in Y direction
#define ACT_SHADOWY		(210*SCALE)	// actor's shadow Y
#define ACT_JUMPSPEED	10			// actor's jump speed (jump height = 10+9+8... = n*(n+1)/2 = 10*11/2 = 55)
#define ACT_DJUMPSPEED	1			// delta jump speed
#define ACT_ENEMYZONE	(30*SCALE)	// Y zone range to search enemies
#define ACT_HIT_DIST	(20*SCALE)	// actor hit distance
#define BLOOD_TIME		(5*SCALE)	// blood time
#define ACT_HIT_FLY		6			// actor hit points - flying
#define ACT_HIT_WALK	3			// actor hit points - walking

// actor
extern int ActInx;			// index of current actor (0=Jill)
extern int ActorPhase;		// actor animation phase
extern int ActorX;			// actor middle X coordinate
extern int ActorY;			// actor middle Y coordinate
extern int ActorW;			// actor image width
extern int ActorH;			// actor image height
extern int ActorMinX, ActorMaxX; // actor's middle min/max X coordinate
extern int ActorMinY, ActorMaxY; // actor's middle min/max Y coordinate
extern int ActorJumpSpeed;	// actor's current jump speed
extern int ActorReload;		// counter to reload actor's weapon
extern int BloodTime;		// blood timer (0 = none)
extern int BloodX, BloodY;	// blood coordinate

extern SCanvas* ImgAct[ACT_NUM];	// images of actors
extern SCanvas* ImgActor;		// image of current actor
extern SCanvas* ImgShadow;	// shadow image
extern SCanvas* ImgBlood;	// blood image

// actor template descriptor
typedef struct {
	const char*	name;		// actor name
	const char*	missile;	// missile name
	Bool		walk;		// walking actor
} sActorTemp;

extern const sActorTemp	ActorTemp[ACT_NUM]; // actor templates
extern sActorTemp	Actor;		// current actor template

// Jill
#define JILL_RUNNUM	8		// Jill run phases
#define JILL_JUMPUP	8		// Jill jump up phase
#define JILL_JUMPDN	9		// Jill jump down phase
#define JILL_STAND	10		// Jill stand phase

// load actor images
BOOL LoadAct();

// activate blood
void SetBlood(int x, int y);

// display blood
void DispBlood();

// shift blood
void ShiftBlood();

// switch actor
void SetActor(int actinx);

// initialize actor on a new game
void InitActor();

// display actor
void DispActor();

// shift actor animation
void ShiftActor();

// falling actor
void FallActor();

// control actor
void CtrlActor();

// hit actor
void HitActor();

