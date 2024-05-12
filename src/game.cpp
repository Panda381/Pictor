
// ****************************************************************************
//
//                                Game
//
// ****************************************************************************

#include "Include.h"

int Level;			// current scene level - 1
int SubFrame;		// sub-frame counter
int CurrentFrame;	// current game frame
int MaxFrame;		// max. game frames

// start first level
void StartGame0()
{
	// reset score
	Score = 0;

	// reset bonuses
	BonusMask = 0;

	// life (0..LIFE_MAX)
	Life = LIFE_INIT;

	// actor 0
	ActInx = 0;

	// initialize new game
	NewGame(STARTLEVEL);
}

// initialize new game (level = 0..)
void NewGame(int level)
{
	// stop all sounds
	StopAllSound();

	// set new level
	Level = level;
	SubFrame = 0;		// sub-frame counter
	CurrentFrame = 0;	// current game frame
	MaxFrame = GAMELEN;	// max. game frames
	BloodTime = 0;		// blood timer (0 = none)

	// initialize random number generator (will be used in InitBonus)
	EnemyRandSeed = Level + 123456;

	// initialize backgrounds on new level
	InitBack();

	// prepare current actor
	InitActor();

	// initialize actor missiles on new level
	InitMissile();

	// initialize enemies
	InitEnemy();

	// initialize enemy bullets on new level
	InitBullet();

	// initialize bonus on start of next level
	InitBonus();

	// initialize points on start of level
	InitPoint();

	// open game start
	GameStart();
}

// display game screen
void DispGame()
{
	// display backgrounds
	DispBack();

	// display actor
	DispActor();

	// display enemies
	EnemyDisp();

	// display explosions
	ExplosionDisp();

	// display bullets
	BulletDisp();

	// display missiles
	MissileDisp();

	// display blood
	DispBlood();

	// display bonus
	BonusDisp();

	// display points
	PointDisp();

	// display header
	DispHeader();

	// display foot
	DispFoot();
}

// shift game
void ShiftGame()
{
	// generate new enemy
	NewEnemy();

	// control actor
	CtrlActor();

	// shift and move enemies, shoot bullets
	EnemyShift();

	// shift explosions
	ExplosionShift();

	// shift bullets
	BulletShift();

	// shift missiles
	MissileShift();

	// hit enemy by missile
	EnemyHit();

	// shift background animation
	ShiftBack();

	// shift actor animation
	ShiftActor();

	// shift blood
	ShiftBlood();

	// shift bonus
	BonusShift();

	// shift points
	PointShift();

	// hit actor
	HitActor();

	// collect bonus
	BonusCollect();

	// shift frame
	SubFrame++;
	if (SubFrame >= SUBFRAMES)
	{
		SubFrame = 0;

		// shift game frame
		CurrentFrame++;

		// end level
#if !DEB_ENDLESS	// 1=endless level
		if (CurrentFrame >= MaxFrame)
		{
			NewGame(Level+1);
		}
#endif
	}

	// game end
	if (Life <= 0) GameEnd();
}
