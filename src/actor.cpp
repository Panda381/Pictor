
// ****************************************************************************
//
//                                  Actors
//
// ****************************************************************************

#include "Include.h"

// actor
int ActInx;			// index of current actor (0=Jill)
int ActorPhase;		// actor animation phase
int ActorX;			// actor middle X coordinate
int ActorY;			// actor middle Y coordinate
int ActorW;			// actor image width
int ActorH;			// actor image height
int ActorMinX, ActorMaxX; // actor's middle min/max X coordinate
int ActorMinY, ActorMaxY; // actor's middle min/max Y coordinate
int ActorJumpSpeed;	// actor's current jump speed
int ActorReload;	// counter to reload actor's weapon
int BloodTime;		// blood timer (0 = none)
int BloodX, BloodY;	// blood coordinate

SCanvas* ImgAct[ACT_NUM];	// images of actors
SCanvas* ImgActor;		// image of current actor
SCanvas* ImgShadow;	// shadow image
SCanvas* ImgBlood;	// blood image

// actor templates
const sActorTemp ActorTemp[ACT_NUM] = {
	// name					missile					walk
	{ "Bara",				"Shurigen",				True,	},
	{ "Bird",				"Seed",					False,	},
	{ "Parrot",				"Berry",				False,	},
	{ "Scarabeus",			"Sand Ball",			True,	},
	{ "Stingray",			"Lightning",			False,	},
	{ "Ice Bird",			"Ice Crystal",			False,	},
	{ "Dragon",				"Fire Ball",			False,	},
	{ "Ghost",				"Screaming",			False,	},
	{ "R2-D2",				"Laser",				True,	},
	{ "UFO",				"Phaser",				False,	},
	{ "Rocket Squirrel",	"Rocket",				False,	},
	{ "Gingerbread Man",	"Sugar Bomb",			True,	},
	{ "Devil Bird",			"Hole of Nothingness",	False,	},
};

sActorTemp		Actor;		// current actor template

// load actor images
BOOL LoadAct()
{
	if (!LoadImg(&ImgAct[0], TGA_00ACT)) return FALSE;
	if (!LoadImg(&ImgAct[1], TGA_01ACT)) return FALSE;
	if (!LoadImg(&ImgAct[2], TGA_02ACT)) return FALSE;
	if (!LoadImg(&ImgAct[3], TGA_03ACT)) return FALSE;
	if (!LoadImg(&ImgAct[4], TGA_04ACT)) return FALSE;
	if (!LoadImg(&ImgAct[5], TGA_05ACT)) return FALSE;
	if (!LoadImg(&ImgAct[6], TGA_06ACT)) return FALSE;
	if (!LoadImg(&ImgAct[7], TGA_07ACT)) return FALSE;
	if (!LoadImg(&ImgAct[8], TGA_08ACT)) return FALSE;
	if (!LoadImg(&ImgAct[9], TGA_09ACT)) return FALSE;
	if (!LoadImg(&ImgAct[10], TGA_10ACT)) return FALSE;
	if (!LoadImg(&ImgAct[11], TGA_11ACT)) return FALSE;
	if (!LoadImg(&ImgAct[12], TGA_12ACT)) return FALSE;

	if (!LoadImg(&ImgShadow, TGA_SHADOW)) return FALSE;
	if (!LoadImg(&ImgBlood, TGA_BLOOD)) return FALSE;

	return TRUE;
}

// activate blood
void SetBlood(int x, int y)
{
	int h = ImgBlood->h/2 + 1;
	if (y < BG_Y_MIN + h) y = BG_Y_MIN + h;
	if (y > BG_Y_MAX - h) y = BG_Y_MAX - h;

	int w = ImgBlood->w/2 + 1;
	if (x < w) x = w;
	if (x > WIDTH - w) x = WIDTH - w;

	BloodX = x;
	BloodY = y;
	BloodTime = BLOOD_TIME;

	if (SoundMode != SOUNDMODE_OFF)
		PlaySound(SOUNDCHAN_ACTORHIT, FailSnd, False, SOUNDVOL_ACTORHIT);
}

// display blood
void DispBlood()
{
	if (BloodTime > 0)
	{
		int w = ImgBlood->w;
		int h = ImgBlood->h;
		DrawImageTrans(ImgBlood, 	// image data and palettes
			0, 0,					// source X, Y
			BloodX - w/2, BloodY - h/2, // destination X, Y
			w, h);				// width, height
	}
}

// shift blood
void ShiftBlood()
{
	if (BloodTime > 0) BloodTime--;
}

// switch actor
void SetActor(int actinx)
{
	// prepare number of actors (including Jill)
#if DEB_ALLACTORS		// 1=all actors are available
	int actnum = ACT_NUM;
#else
	int actnum = Level + 2;
	if (actnum > ACT_NUM) actnum = ACT_NUM;
#endif

	// overflow index
	if (actinx < 0) actinx += actnum;
	if (actinx >= actnum) actinx -= actnum;
	if (actinx < 0) actinx = 0;
	if (actinx >= actnum) actinx = actnum-1;

	// reset animation phase
	ActorPhase = 0;

	// set maximum missile reload (to avoid cheating on actor switch)
	ActorReload = MissileTemp[actinx].reload * SCALE;

	// set new actor
	ActInx = actinx;
	memcpy(&Actor, &ActorTemp[actinx], sizeof(Actor));
	ImgActor = ImgAct[actinx];

	// image size
	int w = ImgActor->w/((actinx == ACT_JILL) ? 11 : 4);
	ActorW = w;
	int h = ImgActor->h;
	ActorH = h;

	// min/max X coordinate
	ActorMinX = ACT_MINX + w/2;
	ActorMaxX = ACT_MAXX - w/2;
	if (ActorX < ActorMinX) ActorX = ActorMinX;
	if (ActorX > ActorMaxX) ActorX = ActorMaxX;

	// min/max Y coordinate
	ActorMinY = ACT_MINY + h/2;
	ActorMaxY = ACT_MAXY - h/2;
	if (ActorY < ActorMinY) ActorY = ActorMinY;
	if (ActorY > ActorMaxY) ActorY = ActorMaxY;

	// not jumping
	ActorJumpSpeed = 0;
}

// initialize actor on a new game
void InitActor()
{
	// set currect actor
	SetActor(ActInx);

	// start position
	ActorX = ActorMinX;
	if (Actor.walk)
		ActorY = ActorMaxY; // Y of walking actor
	else
		ActorY = (ActorMinY + ActorMaxY)/2; // Y of flying actor
}

// display actor
void DispActor()
{
	// display shadow
	int x = ActorX - ImgShadow->w/2;
	if (ActInx == ACT_STINGRAY) x -= 10*SCALE; // correction X for Stingray
	DrawImgShadow(ImgShadow,		// source image
			x,						// destination X
			ACT_SHADOWY);			// destination Y

	// display actor
	int w = ActorW;
	int h = ActorH;
	DrawImageTrans(	ImgActor,	// source image data
			ActorPhase*w,		// source X
			0,					// source Y
			ActorX - w/2,		// destination X
			ActorY - h/2,		// destination Y
			w,					// image width
			h);					// image height
}

// shift actor animation
void ShiftActor()
{
	if (SubFrame != 0) return;

	// actor is moving (if not jumping)
	if (!Actor.walk || (ActorY >= ActorMaxY))
	{
		ActorPhase++;
		if (ActorPhase >= ((ActInx == ACT_JILL) ? JILL_RUNNUM : 4)) ActorPhase = 0;
	}
}

// falling actor
void FallActor()
{
	if (Actor.walk)
	{
		ActorY += ActorJumpSpeed;
		if (ActorY >= ActorMaxY)
		{
			ActorY = ActorMaxY;
			ActorJumpSpeed = 0;
		}
		else
		{
			if (SubFrame == 0)
			{
				ActorJumpSpeed += ACT_DJUMPSPEED;
				if (ActInx == ACT_JILL)
				{
					if (ActorJumpSpeed < 0)
						ActorPhase = JILL_JUMPUP;
					else
						ActorPhase = JILL_JUMPDN;
				}
				else
					ActorPhase = 0;
			}
		}
	}
}

// control actor
void CtrlActor()
{
	// Shooting
#if !DEB_ALLSHOOT					// 1=shoot all the time
	if (KeyShootPressed())
#endif
	{
		// generate missile
		GenMissile();
	}

	// Key left
	if (KeyLeftPressed())
	{
		ActorX -= ACT_SPEEDX;
		if (ActorX < ActorMinX) ActorX = ActorMinX;
	}

	// Key right
	if (KeyRightPressed())
	{
		ActorX += ACT_SPEEDX;
		if (ActorX > ActorMaxX) ActorX = ActorMaxX;
	}

	// Key up
	if (KeyUpPressed())
	{
		// jump
		if (Actor.walk)
		{
			if (ActorY == ActorMaxY) ActorJumpSpeed = -ACT_JUMPSPEED;
		}
		else
		{
			ActorY -= ACT_SPEEDY;
			if (ActorY < ActorMinY) ActorY = ActorMinY;
		}
	}

	// Key down
	if (!Actor.walk && KeyDownPressed())
	{
		ActorY += ACT_SPEEDY;
		if (ActorY > ActorMaxY) ActorY = ActorMaxY;
	}

	// jumping/falling actor
	FallActor();
}

// hit actor
void HitActor()
{
	int i, x, y, dx, dy;

	// hit by bullets
	SCanvas* img;
	sBullet* b = Bullet;
	for (i = 0; i < BULLET_MAX; i++)
	{
		img = b->img;
		if (img != NULL)
		{
			// get bullet coordinates
			x = b->x;
			y = b->y;

			// check distance
			dx = x - ActorX;
			dy = y - ActorY;
			if (dx*dx + dy*dy <= ACT_HIT_DIST*ACT_HIT_DIST)
			{
				// destroy this bullet
				b->img = NULL;

				// decrease life
#if !DEB_IMMORTALITY					// 1=immortality
				Life -= Actor.walk ? ACT_HIT_WALK : ACT_HIT_FLY;
				if (Life < 0) Life = 0;
#endif
				// activate blood
				SetBlood(x, y);
			}
		}
		b++;
	}

	// hit by enemies
	const sEnemyTemp* te;
	sEnemy* e = Enemy;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		te = e->temp;
		if (te != NULL)
		{
			// get enemy coordinates
			x = e->x;
			y = e->y;

			// check distance
			dx = x - ActorX;
			dy = y - ActorY;
			if (dx*dx + dy*dy <= ACT_HIT_DIST*ACT_HIT_DIST)
			{
				// destroy this enemy
				e->temp = NULL;

				// increase score
				Score += EnemyScore[e->inx];
				AddPoint(EnemyScore[e->inx], x, y);

				// decrease life
#if !DEB_IMMORTALITY					// 1=immortality
				Life -= Actor.walk ? ACT_HIT_WALK : ACT_HIT_FLY;
				if (Life < 0) Life = 0;
#endif
				// activate blood
				SetBlood(x, y);
			}
		}
		e++;
	}
}
