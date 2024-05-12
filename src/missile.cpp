
// ****************************************************************************
//
//                               Actor Missiles
//
// ****************************************************************************

#include "Include.h"

// actor missile images
SCanvas* ImgMissile[ACT_NUM];

// actor missile templates
const sMissileTemp MissileTemp[ACT_NUM] = {
	// phasenum		reload	hits		bounce
	{ 4,			30,		10,			True,	},	// 0: shurigen
	{ 1,			6,		10,			True,	},	// 1: seed
	{ 1,			13,		10,			True,	},	// 2: berry
	{ 2,			8,		10,			False,	},	// 3: sand ball
	{ 2,			16,		1,			False,	},	// 4: lightning
	{ 1,			20,		2,			True,	},	// 5: ice crystal
	{ 4,			22,		2,			False,	},	// 6: fire ball
	{ 1,			25,		3,			True,	},	// 7: screaming
	{ 1,			4,		1,			False,	},	// 8: laser
	{ 1,			4,		1,			False,	},	// 9: phaser
	{ 2,			4,		1,			True,	},	// 10: rocket
	{ 4,			22,		1,			True,	},	// 11: sugar bomb
	{ 6,			20,		10,			True,	},	// 12: hole of nothingness
};

// actor missile list
sMissile Missile[MISSILE_MAX];

// load missile images
BOOL LoadMissile()
{
	if (!LoadImg(&ImgMissile[0], TGA_00SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[1], TGA_01SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[2], TGA_02SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[3], TGA_03SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[4], TGA_04SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[5], TGA_05SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[6], TGA_06SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[7], TGA_07SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[8], TGA_08SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[9], TGA_09SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[10], TGA_10SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[11], TGA_11SHOT)) return FALSE;
	if (!LoadImg(&ImgMissile[12], TGA_12SHOT)) return FALSE;

	return TRUE;
}

// initialize actor missiles on new level
void InitMissile()
{
	// clear bullets
	int i;
	for (i = 0; i < MISSILE_MAX; i++) Missile[i].temp = NULL;
	ActorReload = 0;
}

// add new missile
void AddMissile(const sMissileTemp* temp, SCanvas* img, int actinx, int x, int y, int dx, int dy, sEnemy* target)
{
	int i;
	sMissile* m = Missile;
	for (i = 0; i < MISSILE_MAX; i++)
	{
		if (m->temp == NULL)
		{
			m->temp = temp;
			m->img = img;
			m->x = x;
			m->y = y;
			m->w = img->w / temp->phasenum;
			m->h = img->h;
			m->dx = dx;
			m->dy = dy;
			m->actinx = actinx;
			m->phase = 0;
			m->lifetime = (actinx == ACT_STINGRAY) ? MISSILE_LIFETIME_FLASH : MISSILE_LIFETIME_SCREAM;
			m->hits = temp->hits;
			m->target = target;
			break;
		}
		m++;
	}
}

// shift missiles
void MissileShift()
{
	int i;
	const sMissileTemp* temp;
	SCanvas* img;
	sMissile* m = Missile;
	sEnemy* e;
	for (i = 0; i < MISSILE_MAX; i++)
	{
		temp = m->temp;
		if (temp != NULL)
		{
			img = m->img;

			// auto-target
			e = m->target;
			if (e != NULL)
			{
				// enemy was destroyed ... or random lost focus, except Squirrel (which has more accurate targeting system)
				if ((e->temp == NULL) || ((SubFrame == 0) && (RandU16() < 20000) && (m->actinx != ACT_SQUIRREL) && (m->actinx != ACT_GINGER)))
					m->target = NULL;
				else
				{
					// prepare direction
					int dx = e->x - m->x;
					if (dx > 0)
					{
						int dy = e->y - m->y;
						int speed = ((m->actinx == ACT_SQUIRREL) || (m->actinx == ACT_GINGER)) ? MISSILE_SPEEDX_MID : MISSILE_SPEEDX_MAX;

						// direction straight up
						if (-dy >= dx)
						{
							m->dy = -speed;
							m->dx = dx * speed / -dy;
						}
						else
						{
							// straight down
							if (dy >= dx)
							{
								m->dy = speed;
								m->dx = dx * speed / dy;
							}
							else
							{
								m->dx = speed;
								m->dy = dy * speed / dx;
							}
						}
					}
				}
			}			

			// shift missile
			m->x += m->dx;
			m->y += m->dy;

			// missile 03 Scarabeus - falling down
			if ((m->actinx == ACT_SCARABEUS) && (SubFrame == 0))
			{
				m->dy += ACT_DJUMPSPEED;
			}

			// missile deflection from the edges
			int k = BG_Y_MIN + m->h/2 + 1;
			if (m->y < k)
			{
				if (!temp->bounce || (m->dx == 0)) m->temp = NULL; // destroy on sky
				m->dy = - m->dy;
				m->y = k;
			}

			k = BG_Y_MAX - m->h/2 - 1;
			if (m->y > k)
			{
				if (!temp->bounce || (m->dx == 0)) m->temp = NULL; // destroy on earth
				m->dy = - m->dy;
				m->y = k;
			}

			// lifetime
			if ((m->actinx == ACT_STINGRAY) || (m->actinx == ACT_GHOST))
			{
				m->lifetime--;
				if (m->lifetime < 0) m->temp = NULL;
			}

			// shift animation phase
			m->phase++;
			if (m->phase >= temp->phasenum) m->phase = 0;

			// delete missile
			if ((m->x - m->w/2 >= WIDTH) || (m->x < 0)) m->temp = NULL;
		}
		m++;
	}

	if (ActorReload > 0) ActorReload--;
}

// display missiles
void MissileDisp()
{
	int i;
	const sMissileTemp* temp;
	SCanvas* img;
	sMissile* m = Missile;
	for (i = 0; i < MISSILE_MAX; i++)
	{
		temp = m->temp;
		if (temp != NULL)
		{
			img = m->img;

			DrawImageTrans(img,				// image data and palettes
				m->phase * m->w, 0,			// source X, Y
				m->x - m->w/2, m->y - m->h/2, // destination X, Y
				m->w, m->h);				// width, height
		}
		m++;
	}
}

// generate missile
void GenMissile()
{
	// check reload timer
	if (ActorReload > 0) return;
	int actinx = ActInx;
	ActorReload = MissileTemp[actinx].reload * SCALE;

	// prepare start position
	int x = ActorX + ActorW/2;
	int y = ActorY;

	// missile template
	const sMissileTemp* temp = &MissileTemp[actinx];
	SCanvas* img = ImgMissile[actinx];

	// add missiles
	switch (actinx)
	{
	// 00 Jill
	case 0:
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, -MISSILE_SPEEDY_MID, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, 0, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, +MISSILE_SPEEDY_MID, NULL);
		break;

	// 01 Bird
	case 1:
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, 0, NULL);
		break;

	// 02 Parrot
	case 2:
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, -MISSILE_SPEEDY_MIN, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, +MISSILE_SPEEDY_MIN, NULL);
		break;

	// 03 Scarabeus
	case 3:
		AddMissile(temp, img, actinx, x - ActorW/4, y - ActorH/2,
			RandS16MinMax(MISSILE_SPEEDX_MIN, MISSILE_SPEEDX_MID),
			RandS16MinMax(-MISSILE_SPEEDY_MAX, -MISSILE_SPEEDY_MIN), NULL);
		break;

	// 04 Stingray
	case 4:
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH0, 0, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH1, -MISSILE_SPEED_FLASH3, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH2, -MISSILE_SPEED_FLASH2, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH3, -MISSILE_SPEED_FLASH1, NULL);
		AddMissile(temp, img, actinx, x, y, 0, -MISSILE_SPEED_FLASH0, NULL);
		AddMissile(temp, img, actinx, x, y, -MISSILE_SPEED_FLASH3, -MISSILE_SPEED_FLASH1, NULL);
		AddMissile(temp, img, actinx, x, y, -MISSILE_SPEED_FLASH2, -MISSILE_SPEED_FLASH2, NULL);
		AddMissile(temp, img, actinx, x, y, -MISSILE_SPEED_FLASH1, -MISSILE_SPEED_FLASH3, NULL);
		AddMissile(temp, img, actinx, x, y, -MISSILE_SPEED_FLASH0, 0, NULL);
		AddMissile(temp, img, actinx, x, y, -MISSILE_SPEED_FLASH1, MISSILE_SPEED_FLASH3, NULL);
		AddMissile(temp, img, actinx, x, y, -MISSILE_SPEED_FLASH2, MISSILE_SPEED_FLASH2, NULL);
		AddMissile(temp, img, actinx, x, y, -MISSILE_SPEED_FLASH3, MISSILE_SPEED_FLASH1, NULL);
		AddMissile(temp, img, actinx, x, y, 0, MISSILE_SPEED_FLASH0, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH3, MISSILE_SPEED_FLASH1, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH2, MISSILE_SPEED_FLASH2, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH1, MISSILE_SPEED_FLASH3, NULL);
		break;

	// 05 Ice Bird
	case 5:
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, -MISSILE_SPEEDY_MIN, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, 0, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, +MISSILE_SPEEDY_MIN, NULL);
		break;

	// 06 Dragon
	case 6:
	// 07 Ghost
	case 7:
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH2, -MISSILE_SPEED_FLASH2, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH1, -MISSILE_SPEED_FLASH3, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH0, 0, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH1, +MISSILE_SPEED_FLASH3, NULL);
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_FLASH2, +MISSILE_SPEED_FLASH2, NULL);
		break;

	// 08 R2-D2
	case 8:
		y -= ActorH/2;
	// 09 UFO
	case 9:
		{
			int j;
			sEnemy* target = NULL;
			sEnemy* enemy;

			// try to find some enemy as target
			for (j = ENEMY_MAX; j > 0; j--)
			{
				enemy = &Enemy[RandU16Max(ENEMY_MAX-1)];
				if ((enemy->temp != NULL) && (enemy->x > x) && (enemy->x - enemy->w/2 < WIDTH))
				{
					target = enemy;
					break;
				}
			}

			AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, 0, target);
		}
		break;

	// 10 Squirrel
	case 10:
	// 11 Gingerbread Man
	case 11:
		{
			int j;
			sEnemy* target = NULL;
			sEnemy* enemy;

			// try to find some enemy as target
			for (j = ENEMY_MAX*3; j > 0; j--)
			{
				enemy = &Enemy[RandU16Max(ENEMY_MAX-1)];
				if ((enemy->temp != NULL) && (enemy->x > x) && (enemy->x - enemy->w/2 < WIDTH))
				{
					target = enemy;
					break;
				}
			}

			// not found? Try find in order
			if (target == NULL)
			{
				enemy = Enemy;
				for (j = ENEMY_MAX; j > 0; j--)
				{
					if ((enemy->temp != NULL) && (enemy->x > x) && (enemy->x - enemy->w/2 < WIDTH))
					{
						target = enemy;
						break;
					}
					enemy++;
				}
			}

			AddMissile(temp, img, actinx, x, y, MISSILE_SPEEDX_MID, 0, target);
		}
		break;

	// 12 Devil Bird
	case 12:
		AddMissile(temp, img, actinx, x, y, MISSILE_SPEED_BLACKHOLE, 0, NULL);
		break;
	}

	// sound
	if (SoundMode != SOUNDMODE_OFF)
		PlaySound(SOUNDCHAN_MISSILE, MissileSnd[actinx], False, SOUNDVOL_MISSILE);
}
