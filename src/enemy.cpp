
// ****************************************************************************
//
//                               Enemies
//
// ****************************************************************************

#include "Include.h"

// move templates
const sMoveSeg Move1[] = { MOVEL(0), };		// straight left
const sMoveSeg Move1U[] = { MOVEL_SU(0), };	// straight left, slow up
const sMoveSeg Move1D[] = { MOVEL_SD(0), };	// straight left, slow down

const sMoveSeg Move2[] = { MOVEL(20), MOVELD(30), MOVEL(5), MOVELU(10), MOVEL(0), };	// left down
const sMoveSeg Move3[] = { MOVEL(20), MOVELU(30), MOVEL(5), MOVELD(10), MOVEL(0), };	// left up

const sMoveSeg Move4[] = { MOVEL(10), MOVELU(10), MOVEL(5), MOVELD(20), MOVEL(5), MOVELU(20), MOVEL(5), MOVELD(10), MOVEL(0), };	// saw
const sMoveSeg Move5[] = { MOVEL(10), MOVELD(10), MOVEL(5), MOVELU(20), MOVEL(5), MOVELD(20), MOVEL(5), MOVELU(10), MOVEL(0), };	// saw 2
const sMoveSeg Move6[] = { MOVEL(40), MOVEU(20), MOVEL(0), };	// up
const sMoveSeg Move7[] = { MOVEL(40), MOVED(20), MOVEL(0), };	// down
const sMoveSeg* Moves[] = { Move1, Move1U, Move1D,   Move2, Move3,   Move4, Move5, Move6, Move7 };

// enemy images
SCanvas* ImgEnemy[ENEMY_NUM];

// enemy score
const s8 EnemyScore[3] = { 10, 20, 50 };

// current enemy template
sEnemyTemp EnemyNow[ENEMY_LEVNUM];

// current enemy images
SCanvas* ImgEnemyNow[ENEMY_LEVNUM];

// width of image phase
int ImgEnemyWNow[ENEMY_LEVNUM];

// enemies
sEnemy Enemy[ENEMY_MAX];

// explosions
sExplosion Explosion[EXPLOSIONS_MAX];
SCanvas* ImgExplosion;	// explosion image
int ImgExplosionW; // width of explosion image

// enemy templates
const sEnemyTemp EnemyTemp[ENEMY_NUM] = {
	// name					animmax		animshift	animmask
// 1:
	{ "Fly",				1,			0,			1,	},
	{ "Wasp",				1,			0,			1,	},
	{ "Hornet",				1,			0,			1,	},
// 2:
	{ "Dragonfly",			1,			0,			1,	},
	{ "Moth",				1,			0,			1,	},
	{ "Kestrel",			3,			1,			1,	},
// 3:
	{ "Maggot",				0,			0,			0,	},
	{ "Eagle",				3,			1,			1,	},
	{ "Antman",				3,			1,			1,	},
// 4:
	{ "Fish",				3,			1,			1,	},
	{ "Anglerfish",			3,			1,			1,	},
	{ "Shark",				3,			1,			1,	},
// 5:
	{ "Penguin",			1,			0,			1,	},
	{ "Polar Bear",			3,			1,			1,	},
	{ "Snowflake",			0,			0,			0,	},
// 6:
	{ "Phoenix",			3,			1,			1,	},
	{ "Fire Ball",			3,			1,			1,	},
	{ "Evil",				3,			1,			1,	},
// 7:
	{ "Pumpkin",			3,			1,			1,	},
	{ "Zombie",				0,			0,			0,	},
	{ "Death",				3,			1,			1,	},
// 8:
	{ "Camera",				0,			0,			0,	},
	{ "Explorer",			3,			1,			1,	},
	{ "War Dron",			0,			0,			0,	},
// 9:
	{ "Starship Enterprise", 0,			0,			0,	},
	{ "Klingon Warship",	0,			0,			0,	},
	{ "Death Star",			0,			0,			0,	},
// 10:
	{ "Alien",				1,			0,			1,	},
	{ "Xenomorph",			1,			0,			1,	},
	{ "Meteor",				3,			1,			1,	},
// 11:
	{ "Candy Cane",			3,			1,			1,	},
	{ "Lollipop",			3,			1,			1,	},
	{ "Fat Man",			3,			1,			1,	},
// 12:
	{ "Cloud",				0,			0,			0,	},
	{ "Star",				0,			0,			0,	},
	{ "Moon",				0,			0,			0,	},
};

// load enemy images
BOOL LoadEnemy()
{
	if (!LoadImg(&ImgEnemy[0], TGA_01EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[1], TGA_01EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[2], TGA_01EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[3], TGA_02EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[4], TGA_02EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[5], TGA_02EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[6], TGA_03EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[7], TGA_03EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[8], TGA_03EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[9], TGA_04EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[10], TGA_04EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[11], TGA_04EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[12], TGA_05EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[13], TGA_05EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[14], TGA_05EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[15], TGA_06EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[16], TGA_06EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[17], TGA_06EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[18], TGA_07EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[19], TGA_07EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[20], TGA_07EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[21], TGA_08EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[22], TGA_08EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[23], TGA_08EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[24], TGA_09EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[25], TGA_09EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[26], TGA_09EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[27], TGA_10EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[28], TGA_10EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[29], TGA_10EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[30], TGA_11EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[31], TGA_11EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[32], TGA_11EN3)) return FALSE;

	if (!LoadImg(&ImgEnemy[33], TGA_12EN1)) return FALSE;
	if (!LoadImg(&ImgEnemy[34], TGA_12EN2)) return FALSE;
	if (!LoadImg(&ImgEnemy[35], TGA_12EN3)) return FALSE;

	if (!LoadImg(&ImgExplosion, TGA_EXPLOSION)) return FALSE;
	ImgExplosionW = ImgExplosion->w / EXPLOSION_PHASES;

	return TRUE;
}

// initialize enemies on new level
void InitEnemy()
{
	// prepare enemy templates
	int i = BackInx * ENEMY_LEVNUM;
	int j;
	for (j = 0; j < ENEMY_LEVNUM; j++)
	{
		memcpy(&EnemyNow[j], &EnemyTemp[i], sizeof(sEnemyTemp));
		ImgEnemyNow[j] = ImgEnemy[i];
		ImgEnemyWNow[j] = ImgEnemy[i]->w / (EnemyTemp[i].animmask + 1);
		i++;
	}

	// clear enemies
	for (i = 0; i < ENEMY_MAX; i++) Enemy[i].temp = NULL;

	// clear explosions
	for (i = 0; i < EXPLOSIONS_MAX; i++) Explosion[i].anim = EXPLOSION_PHASES;
}

// add new enemy (temp = enemy template index 0..2)
void AddEnemy(const sEnemyTemp* temp, SCanvas* img, int w, int x, int y, const sMoveSeg* move, int inx)
{
	int i;
	sEnemy* e = Enemy;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		if (e->temp == NULL)
		{
			e->temp = temp;
			e->img = img;
			e->w = w;
			e->x = x;
			e->y = y;
			e->anim = 0;
			e->move = move;
			e->step = 0;
			e->inx = inx;
			break;
		}
		e++;
	}
}

// add explosion
void AddExplosion(int x, int y)
{
	int i;
	sExplosion* e = Explosion;
	for (i = 0; i < EXPLOSIONS_MAX; i++)
	{
		if (e->anim >= EXPLOSION_PHASES) // if entry is not used
		{
			e->anim = 0;
			e->x = x;
			e->y = y;
			break;
		}
		e++;
	}
}

// generate new enemy
void NewEnemy()
{
	if (SubFrame != 0) return;

	// prepare current randomness
	int gen = (int)(((ENEMY_GEN_END - ENEMY_GEN_BEG)*CurrentFrame/MaxFrame + ENEMY_GEN_BEG) * powf(ENEMY_GEN_LEVEL, (float)Level));

	// check randomness
	if (EnemyRand() <= (u16)gen)
	{
		// randomness of enemy 3 (range 0..65535, starting at 1/2 of the level)
		gen = 0;
		if (CurrentFrame*2 >= MaxFrame) gen = (CurrentFrame*2 - MaxFrame)*65536/MaxFrame;

		// generate enemy 3
		int inx;
		if (EnemyRand() < gen)
			inx = 2;
		else
		{
			// randomness of enemy 2 (range 0..65535)
			gen = CurrentFrame*50000/MaxFrame;

			// generate enemy 2, or 1 otherwise
			if (EnemyRand() < gen)
				inx = 1;
			else
				inx = 0;
		}

		// enemy template
		const sEnemyTemp* e = &EnemyNow[inx];
		SCanvas* can = ImgEnemyNow[inx];
		int w = ImgEnemyWNow[inx];

		// enemy coordinate Y
		int ymin = BG_Y_MIN + can->h/2 + ENEMY_BORDERY;
		int ymax = BG_Y_MAX - can->h/2 - ENEMY_BORDERY;
		int y = EnemyRandMinMax(ymin, ymax);

		// move
		const sMoveSeg* move;
		int n;
		if (inx == 0) // enemy 1: move straight left
			n = 2;
		else if (inx == 1) // enemy 2: move diagonally
			n = 4;
		else // enemy 3: random movement
			n = count_of(Moves)-1;

		move = Moves[EnemyRandMax(n)];

		// add enemy to the list
		AddEnemy(e, can, w, WIDTH + w/2 + 2, y, move, inx);
	}
}

// shift enemies, shoot bullets
void EnemyShift()
{
	int i, a;
	const sEnemyTemp* temp;
	SCanvas* img;
	sEnemy* e = Enemy;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		temp = e->temp;
		if (temp != NULL)
		{
			img = e->img;

			// shift enemy animation
			if (SubFrame == 0)
			{
				a = e->anim + 1;
				if (a > temp->animmax) a = 0;
				e->anim = a;
			}

			// move enemy
			const sMoveSeg* move = e->move;
			e->x += move->dx;
			e->y += move->dy;

			// limit Y coordinate
			int ymin = BG_Y_MIN + img->h/2 + ENEMY_BORDERY;
			int ymax = BG_Y_MAX - img->h/2 - ENEMY_BORDERY;

			if (e->y < ymin)
			{
				e->y = ymin;
				e->step = move->steps; // next move segment
				if (move == Move1U) e->move = Move1D; // flip direction
			}

			if (e->y > ymax)
			{
				e->y = ymax;
				e->step = move->steps; // next move segment
				if (move == Move1D) e->move = Move1U; // flip direction
			}

			// shift move step
			if (move->steps != 0)
			{
				e->step++;

				// next move segment
				if (e->step >= move->steps)
				{
					e->step = 0;
					e->move = move + 1;
				}
			}

			// delete enemy
			if (e->x + e->w/2 <= -2)
				e->temp = NULL;
			else
			{
				// generate bullet
				if ((SubFrame == 0) && (RandU16() < ENEMY_GEN_BUL))
				{
					AddBullet(ImgBullet[e->inx], e->x - e->w/2, e->y);
				}
			}
		}
		e++;
	}
}

// shift explosions
void ExplosionShift()
{
	if (SubFrame != 0) return;

	int i;
	sExplosion* e = Explosion;
	for (i = 0; i < EXPLOSIONS_MAX; i++)
	{
		if (e->anim < EXPLOSION_PHASES)
		{
			e->anim++;
		}
		e++;
	}
}

// display enemies
void EnemyDisp()
{
	int i;
	const sEnemyTemp* temp;
	SCanvas* img;
	sEnemy* e = Enemy;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		temp = e->temp;
		img = e->img;
		if (temp != NULL)
		{
			DrawImageTrans(img,		// image data and palettes
				((e->anim >> temp->animshift) & temp->animmask)*e->w, 0,	// source X, Y
				e->x - e->w/2, e->y - img->h/2, // destination X, Y
				e->w, img->h);		// width, height
		}
		e++;
	}
}

// display explosions
void ExplosionDisp()
{
	int i;
	sExplosion* e = Explosion;
	for (i = 0; i < EXPLOSIONS_MAX; i++)
	{
		if (e->anim < EXPLOSION_PHASES)
		{
			DrawImageTrans(ImgExplosion,	// image data and palettes
				e->anim * ImgExplosionW, 0,	// source X, Y
				e->x - ImgExplosionW/2, e->y - ImgExplosion->h/2, // destination X, Y
				ImgExplosionW, ImgExplosion->h); // width, height
		}
		e++;
	}
}

// hit enemy by missile
void EnemyHit()
{
	int i, j, k, xm, ym, xe, ye, dx, dy, dx2, dy2;
	const sMissileTemp* tm;
	const sEnemyTemp* te;
	sEnemy* e;
	sMissile* m;
	sBullet* b;

	// loop missiles
	m = Missile;
	for (i = 0; i < MISSILE_MAX; i++)
	{
		tm = m->temp;
		if (tm != NULL)
		{
			// get missile coordinates
			xm = m->x;
			ym = m->y;

			// loop enemies
			e = Enemy;
			for (j = 0; j < ENEMY_MAX; j++)
			{
				te = e->temp;
				if ((te != NULL) && (e->x - e->w/2 < WIDTH)) // cannot shot target out of screen
				{
					// get enemy coordinates
					xe = e->x;
					ye = e->y;

					// check distance
					dx = xe - xm;
					dy = ye - ym;
					if (dx*dx + dy*dy <= ENEMY_HIT_DIST*ENEMY_HIT_DIST)
					{
						// destroy this enemy
						e->temp = NULL;

						// increase score
						Score += EnemyScore[e->inx];
						AddPoint(EnemyScore[e->inx], xe, ye);

						// serve black hole
						if (m->actinx == ACT_DEVILBIRD)
						{
							// sound of suck
							if (SoundMode != SOUNDMODE_OFF)
								PlaySound(SOUNDCHAN_ENEMYHIT, Blackhole2Snd, False, SOUNDVOL_SUCK);
						}
						else
						{
							// create explosion
							AddExplosion(xe, ye);

							// sugar bomb
							if (m->actinx == ACT_GINGER)
							{
								// loop enemies
								for (k = 0; k < ENEMY_MAX; k++)
								{
									if (Enemy[k].temp != NULL)
									{
										dx2 = Enemy[k].x - xe;
										dy2 = Enemy[k].y - ye;
										if (dx2*dx2 + dy2*dy2 <= SUGAR_HIT_RADIUS*SUGAR_HIT_RADIUS)
										{
											// destroy this enemy
											Enemy[k].temp = NULL;

											// increase score
											Score += EnemyScore[Enemy[k].inx];
											AddPoint(EnemyScore[Enemy[k].inx], Enemy[k].x, Enemy[k].y);

											// create explosion
											AddExplosion(Enemy[k].x, Enemy[k].y);
										}
									}
								}
							}

							// sound of explosion
							if (SoundMode != SOUNDMODE_OFF)
								PlaySound(SOUNDCHAN_ENEMYHIT, EnemyHitSnd, False, SOUNDVOL_ENEMYHIT);
						}

						// destroy missile
						m->hits--;
						if (m->hits <= 0)
						{
							m->temp = NULL;
							break;
						}
					}
				}

				// next enemy
				e++;
			}

			// black hole - catch bullets
			if (m->actinx == ACT_DEVILBIRD)
			{
				// loop bullets
				b = Bullet;
				for (j = 0; j < BULLET_MAX; j++)
				{
					// check if bullet is valid
					if (b->img != NULL)
					{
						// get bullet coordinates
						xe = b->x;
						ye = b->y;

						// check distance
						dx = xe - xm;
						dy = ye - ym;
						if (dx*dx + dy*dy <= ENEMY_HIT_DIST*ENEMY_HIT_DIST)
						{
							// destroy this bullet
							b->img = NULL;

							// sound of suck
							if (SoundMode != SOUNDMODE_OFF)
								PlaySound(SOUNDCHAN_ENEMYHIT, Blackhole2Snd, False, SOUNDVOL_SUCK);
						}
					}

					// next bullet
					b++;
				}
			}
		}
		
		// next missile
		m++;
	}
}
