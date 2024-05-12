
// ****************************************************************************
//
//                                 Enemy bullets
//
// ****************************************************************************

#include "Include.h"

// enemy bullet images
SCanvas* ImgBullet[BULLET_TEMP_NUM];

// enemy bullet list
sBullet Bullet[BULLET_MAX];

// load bullet images
BOOL LoadBullet()
{
	if (!LoadImg(&ImgBullet[0], TGA_BULLET1)) return FALSE;
	if (!LoadImg(&ImgBullet[1], TGA_BULLET2)) return FALSE;
	if (!LoadImg(&ImgBullet[2], TGA_BULLET3)) return FALSE;

	return TRUE;
}

// initialize enemy bullets on new level
void InitBullet()
{
	// clear bullets
	int i;
	for (i = 0; i < BULLET_MAX; i++) Bullet[i].img = NULL;
}

// add new bullet
void AddBullet(SCanvas* img, int x, int y)
{
	int i;
	sBullet* b = Bullet;
	for (i = 0; i < BULLET_MAX; i++)
	{
		if (b->img == NULL)
		{
			b->img = img;
			b->x = x;
			b->y = y;
			break;
		}
		b++;
	}
}

// shift bullets
void BulletShift()
{
	int i;
	SCanvas* img;
	sBullet* b = Bullet;
	for (i = 0; i < BULLET_MAX; i++)
	{
		img = b->img;
		if (img != NULL)
		{
			// shift bullet
			b->x -= BULLET_SPEED;

			// delete bullet
			if (b->x + img->w/2 <= 0) b->img = NULL;
		}
		b++;
	}
}

// display bullets
void BulletDisp()
{
	int i;
	SCanvas* img;
	sBullet* b = Bullet;
	for (i = 0; i < BULLET_MAX; i++)
	{
		img = b->img;
		if (img != NULL)
		{
			DrawImageTrans(img,			// image data and palettes
				0, 0,					// source X, Y
				b->x - img->w/2, b->y - img->h/2, // destination X, Y
				img->w, img->h);		// width, height
		}
		b++;
	}
}
