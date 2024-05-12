
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                 Canvas Drawing                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "Include.h"

SCanvas* Canvas = NULL;		// draw canvas
SCanvas* Font = NULL;		// font 24x48
SCanvas* FontSmall = NULL;	// small font 8x16

///////////////////////////////////////////////////////////////////////////////
// Draw point

void DrawPoint(int x, int y, u32 col)
{
	SCanvas* dst = Canvas;

	int w = dst->w;
	if (((u32)x >= (u32)w) || ((u32)y >= (u32)dst->h)) return;

	u32* d = (u32*)&dst->d[(x + y*w)*3];
	*d = (*d & 0xff000000) | col;
}

///////////////////////////////////////////////////////////////////////////////
// vertical flip canvas

void VFlipCanvas(SCanvas* c)
{
	int wb = c->w * c->c;
	u8* s1 = c->d;
	int h = c->h;
	u8* s2 = s1 + wb*(h-1);

	int i, j, k;
	for (i = h/2; i > 0; i--)
	{
		for (j = wb/4; j > 0; j--)
		{
			k = *(u32*)s1;
			*(u32*)s1 = *(u32*)s2;
			*(u32*)s2 = k;
			s1 += 4;
			s2 += 4;
		}

		for (j = wb & 3; j > 0; j--)
		{
			k = *s1;
			*s1 = *s2;
			*s2 = (u8)k;
			s1++;
			s2++;
		}
		s2 -= 2*wb;
	}
}

///////////////////////////////////////////////////////////////////////////////
// flip RGB and BGR
/*
void RGBflipBGR(SCanvas* c)
{
	int b = c->c;
	u8* d = c->d;
	int i;
	u8 k;
	for (i = c->w * c->h; i > 0; i--)
	{
		k = d[0];
		d[0] = d[2];
		d[2] = k;
		d += b;
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
// draw horizontal line BGR without alpha

void DrawHLine(int x, int y, int w, u32 col)
{
	SCanvas* dst = Canvas;

	// check destination
	if ((DWORD)y >= (DWORD)dst->h) return;
	if (x < 0)
	{
		w += x;
		x = 0;
	}
	if (x + w > dst->w) w = dst->w - x;
	if (w <= 0) return;

	// destination without alpha
	u8* d = &dst->d[(x + dst->w*y)*dst->c];
	col &= 0xffffff;
	for (; w > 1; w--)
	{
		*(u32*)d = col;
		d += 3;
	}
	*(u32*)d = (*(u32*)d & 0xff000000) | col;

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// draw vertical line BGR without alpha

void DrawVLine(int x, int y, int h, u32 col)
{
	SCanvas* dst = Canvas;

	// check destination
	if ((DWORD)x >= (DWORD)dst->w) return;
	if (y < 0)
	{
		h += y;
		y = 0;
	}
	if (y + h > dst->h) h = dst->h - y;
	if (h <= 0) return;

	// destination without alpha
	int dw = dst->w;
	u8* d = &dst->d[(x + dw*y)*dst->c];
	dw *= dst->c;
	col &= 0xffffff;
	for (; h > 0; h--)
	{
		*(u32*)d = (*(u32*)d & 0xff000000) | col;
		d += dw;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// draw frame

void DrawFrame(int x, int y, int w, int h, int thick, u32 col)
{
	for (; thick > 0; thick--)
	{
		DrawHLine(x, y, w, col);
		DrawHLine(x, y + h - 1, w, col);
		DrawVLine(x, y + 1, h - 2, col);
		DrawVLine(x + w - 1, y + 1, h - 2, col);

		x++;
		y++;
		w -= 2;
		h -= 2;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// draw horizontal dashed line BGR without alpha
/*
void DrawHDash(SCanvas* dst, int x, int y, int w, u32 col1, u32 col2)
{
	// check destination
	if ((DWORD)y >= (DWORD)dst->h) return;
	if (x < 0)
	{
		w += x;
		x = 0;
	}
	if (x + w > dst->w) w = dst->w - x;
	if (w <= 0) return;

	// destination without alpha
	u8* d = &dst->d[(x + dst->w*y)*dst->c];
	col1 &= 0xffffff;
	col2 &= 0xffffff;
	for (; w > 1; w--)
	{
		*(u32*)d = (((x+y) & 2) == 0) ? col1 : col2;
		d += 3;
		x++;
	}
	*(u32*)d = (*(u32*)d & 0xff000000) | ((((x+y) & 2) == 0) ? col1 : col2);
}
*/
///////////////////////////////////////////////////////////////////////////////
// draw vertical dashed line BGR without alpha
/*
void DrawVDash(SCanvas* dst, int x, int y, int h, u32 col1, u32 col2)
{
	// check destination
	if ((DWORD)x >= (DWORD)dst->w) return;
	if (y < 0)
	{
		h += y;
		y = 0;
	}
	if (y + h > dst->h) h = dst->h - y;
	if (h <= 0) return;

	// destination without alpha
	int dw = dst->w;
	u8* d = &dst->d[(x + dw*y)*dst->c];
	dw *= dst->c;
	col1 &= 0xffffff;
	col2 &= 0xffffff;
	for (; h > 0; h--)
	{
		*(u32*)d = (*(u32*)d & 0xff000000) | ((((x+y) & 2) == 0) ? col1 : col2);
		d += dw;
		y++;
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
// draw dashed frame
/*
void DrawDash(SCanvas* dst, int x, int y, int w, int h, u32 col1, u32 col2)
{
	DrawHDash(dst, x, y, w, col1, col2);
	DrawHDash(dst, x, y + h - 1, w, col1, col2);
	DrawVDash(dst, x, y + 1, h - 2, col1, col2);
	DrawVDash(dst, x + w - 1, y + 1, h - 2, col1, col2);
}
*/
///////////////////////////////////////////////////////////////////////////////
// draw rectangle BGR without alpha

void DrawRect(int x, int y, int w, int h, u32 col)
{
	SCanvas* dst = Canvas;

	// limit destination
	if (x < 0)
	{
		w += x;
		x = 0;
	}
	if (x + w > dst->w) w = dst->w - x;
	if (y < 0)
	{
		h += y;
		y = 0;
	}
	if (y + h > dst->h) h = dst->h - y;
	if ((w <= 0) || (h <= 0)) return;

	// destination without alpha
	int dw = dst->w;
	u8* d = &dst->d[(x + dw*y)*dst->c];
	dw = (dw - w)*dst->c;
	col &= 0xffffff;
	int w2;
	dw += dst->c;
	for (; h > 0; h--)
	{
		for (w2 = w; w2 > 1; w2--)
		{
			*(u32*)d = col;
			d += 3;
		}
		*(u32*)d = (*(u32*)d & 0xff000000) | col;
		d += dw;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// clear screen

void DrawClear()
{
	DrawRect(0, 0, WIDTH, HEIGHT, COL_BLACK);
}

///////////////////////////////////////////////////////////////////////////////
// draw arrow
/*
#define DRAWARR(x,y)  *(int*)&b[(w*(y)+(x))*3] = col
#define DRAWARR2(x,y) *(int*)&b[(w*(y)+(x))*3] = col | (*(int*)&b[(w*(y)+(x))*3] & 0xff000000)

void DrawArrow(SCanvas* dst, int x, int y, u32 col)
{
	int w = dst->w;
	u8* b = &dst->d[(x + w*y)*3];

	DRAWARR2(0,0);
	DRAWARR(0,1); DRAWARR2(1,1);
	DRAWARR(0,2); DRAWARR(1,2); DRAWARR2(2,2);
	DRAWARR(0,3); DRAWARR(1,3); DRAWARR(2,3); DRAWARR2(3,3);
	DRAWARR(0,4); DRAWARR(1,4); DRAWARR(2,4); DRAWARR(3,4); DRAWARR2(4,4);
	DRAWARR(0,5); DRAWARR(1,5); DRAWARR(2,5); DRAWARR2(3,5);
	DRAWARR(0,6); DRAWARR(1,6); DRAWARR2(2,6);
	DRAWARR(0,7); DRAWARR2(1,7);
	DRAWARR2(0,8);
}
*/
///////////////////////////////////////////////////////////////////////////////
// draw image

//void DrawImage(SCanvas* dst, SCanvas* src, int x, int y, int h /*= 0*/)
/*
{
	int c = src->c;
	if (c == 4)
	{
		int dw = dst->w;
		u8* d = &dst->d[(x + dw*y)*3];
		dw *= 3;
		int w = src->w;
		u8* s = src->d;
		if (h == 0) h = src->h;
		int x;
		u8 a;
		dw -= w*3;
		for (; h > 0; h--)
		{
			for (x = 0; x < w; x++)
			{
				a = s[3];
				d[0] = (s[0]*a + d[0]*(255-a))/255;
				d[1] = (s[1]*a + d[1]*(255-a))/255;
				d[2] = (s[2]*a + d[2]*(255-a))/255;
				s += 4;
				d += 3;
			}
			d += dw;
		}
	}
	else
	{
		int dw = dst->w;
		u8* d = &dst->d[(x + dw*y)*3];
		dw *= 3;
		int w = src->w * 3;
		u8* s = src->d;
		if (h == 0) h = src->h;
		for (; h > 0; h--)
		{
			memcpy(d, s, w);
			d += dw;
			s += w;
		}
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
// draw image cut
/*
void DrawImageCut(SCanvas* dst, SCanvas* src, int dx, int dy, int sx, int sy, int w, int h)
{
	int dw = dst->w;
	int sw = src->w; 
	u8* d = &dst->d[(dx + dw*dy)*3];
	u8* s = &src->d[(sx + sw*sy)*3];
	dw *= 3;
	sw *= 3;
	w *= 3;
	for (; h > 0; h--)
	{
		memcpy(d, s, w);
		d += dw;
		s += sw;
	}
}
*/

///////////////////////////////////////////////////////////////////////////////
// Draw image (source and destination must have same format)

void DrawImage(SCanvas* src, int xs, int ys, int xd, int yd, int w, int h)
{
	SCanvas* canvas = Canvas;

	// must have same format
	u8 c = canvas->c;
	if (c != src->c) return;

	// limit coordinates X
	if (xd < 0)
	{
		w += xd;
		xs -= xd;
		xd = 0;
	}

	if (xs < 0)
	{
		w += xs;
		xd -= xs;
		xs = 0;
	}

	if (xd + w > canvas->w) w = canvas->w - xd;
	if (xs + w > src->w) w = src->w - xs;
	if (w <= 0) return;

	// limit coordinates Y
	if (yd < 0)
	{
		h += yd;
		ys -= yd;
		yd = 0;
	}

	if (ys < 0)
	{
		h += ys;
		yd -= ys;
		ys = 0;
	}

	if (yd + h > canvas->h) h = canvas->h - yd;
	if (ys + h > src->h) h = src->h - ys;
	if (h <= 0) return;

	int wbd = canvas->w*c;
	int wbs = src->w*c;
	u8* d = canvas->d + xd*c + yd*wbd;
	u8* s = src->d + xs*c + ys*wbs;
	for (; h > 0; h--)
	{
		memcpy(d, s, w*c);
		d += wbd;
		s += wbs;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Draw image transparent

void DrawImageTrans(SCanvas* src, int xs, int ys, int xd, int yd, int w, int h)
{
	SCanvas* canvas = Canvas;

	// check format
	if ((canvas->c != 3) || (src->c != 4)) return;

	// limit coordinates X
	if (xd < 0)
	{
		w += xd;
		xs -= xd;
		xd = 0;
	}

	if (xs < 0)
	{
		w += xs;
		xd -= xs;
		xs = 0;
	}

	if (xd + w > canvas->w) w = canvas->w - xd;
	if (xs + w > src->w) w = src->w - xs;
	if (w <= 0) return;

	// limit coordinates Y
	if (yd < 0)
	{
		h += yd;
		ys -= yd;
		yd = 0;
	}

	if (ys < 0)
	{
		h += ys;
		yd -= ys;
		ys = 0;
	}

	if (yd + h > canvas->h) h = canvas->h - yd;
	if (ys + h > src->h) h = src->h - ys;
	if (h <= 0) return;

	int wbd = canvas->w*3;
	int wbs = src->w*4;
	u8* d = canvas->d + xd*3 + yd*wbd;
	u8* s = src->d + xs*4 + ys*wbs;

	wbd -= w*3;
	wbs -= w*4;

	for (; h > 0; h--)
	{
		int i;
		for (i = w; i > 0; i--)
		{
			u8 b = s[0];
			u8 g = s[1];
			u8 r = s[2];
			u8 a = s[3];

			if (a >= 192)
			{
				d[0] = b;
				d[1] = g;
				d[2] = r;
			}

			s += 4;
			d += 3;
		}

		d += wbd;
		s += wbs;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Draw image shadow

void DrawImgShadow(SCanvas* src, int xd, int yd)
{
	SCanvas* canvas = Canvas;
	int w = src->w;
	int h = src->h;
	int xs = 0;
	int ys = 0;

	// limit coordinates X
	if (xd < 0)
	{
		w += xd;
		xs -= xd;
		xd = 0;
	}

	if (xs < 0)
	{
		w += xs;
		xd -= xs;
		xs = 0;
	}

	if (xd + w > canvas->w) w = canvas->w - xd;
	if (xs + w > src->w) w = src->w - xs;
	if (w <= 0) return;

	// limit coordinates Y
	if (yd < 0)
	{
		h += yd;
		ys -= yd;
		yd = 0;
	}

	if (ys < 0)
	{
		h += ys;
		yd -= ys;
		ys = 0;
	}

	if (yd + h > canvas->h) h = canvas->h - yd;
	if (ys + h > src->h) h = src->h - ys;
	if (h <= 0) return;

	int wbd = canvas->w*3;
	int wbs = src->w*3;
	u8* d = canvas->d + xd*3 + yd*wbd;
	u8* s = src->d + xs*3 + ys*wbs;
	wbd -= w*3;
	wbs -= w*3;
	int i;
	for (; h > 0; h--)
	{
		for (i = w; i > 0; i--)
		{
			if (*s < 128)
			{
				d[0] >>= 1;
				d[1] >>= 1;
				d[2] >>= 1;
			}
			d += 3;
			s += 3;
		}
		d += wbd;
		s += wbs;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// draw image with transparency
/*
void DrawImageTrans(SCanvas* dst, SCanvas* src, int dx, int dy, int sx, int sy, int w, int h, u32 transcol)
{
	int dw = dst->w;
	int sw = src->w; 
	u8* d = &dst->d[(dx + dw*dy)*3];
	u8* s = &src->d[(sx + sw*sy)*3];
	dw *= 3;
	sw *= 3;
	w *= 3;
	int i;
	transcol &= 0xffffff;
	for (; h > 0; h--)
	{
		for (i = w/3; i > 0; i--)
		{
			if (transcol != (*(u32*)s & 0xffffff))
			{
				d[0] = s[0];
				d[1] = s[1];
				d[2] = s[2];
			}
			d += 3;
			s += 3;
		}
		d += dw - w;
		s += sw - w;
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
// draw line
/*
void DrawLine(SCanvas* dst, int x1, int y1, int x2, int y2, u32 col)
{
	// deltas
	int dx = x2 - x1;
	int dy = y2 - y1;
	u8* b = &dst->d[(x1 + dst->w*y1)*3];

	// one pixel
	if ((dx == 0) && (dy == 0))
	{
		*(int*)b = col | (*(int*)b & 0xff000000);
		return;
	}

	int tx = (dx < 0) ? -1 : 1;
	int ty = (dy < 0) ? -1 : 1;

	// X is more significant
	if (abs(dx) > abs(dy))
	{
		dy = abs(dy)*0x10000/abs(dx); 
		int y = 0x8000;
		ty *= dst->w*3;
		
		for (; x1 != x2; x1 += tx)
		{
			*(int*)b = col | (*(int*)b & 0xff000000);
			b += 3*tx;

			y += dy;
			if (y >= 0x10000)
			{
				y -= 0x10000;
				b += ty;
			}
		}

		*(int*)b = col | (*(int*)b & 0xff000000);
	}

	// Y is more significant
	else
	{
		dx = abs(dx)*0x10000/abs(dy); 
		int x = 0x8000;
		int ty2 = ty*dst->w*3;
		tx *= 3;
		
		for (; y1 != y2; y1 += ty)
		{
			*(int*)b = col | (*(int*)b & 0xff000000);
			b += ty2;

			x += dx;
			if (x >= 0x10000)
			{
				x -= 0x10000;
				b += tx;
			}
		}

		*(int*)b = col | (*(int*)b & 0xff000000);
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
// decode picture from memory (returns NULL on error; returns Canvas with ref=1)
/*
SCanvas* ImgDecode(const u8* buf, int size)
{
	return TGADecode(buf, size);
}
*/

///////////////////////////////////////////////////////////////////////////////
// Draw character, 8*SCALE=24 pixels width, 16*SCALE=48 pixels height, transparent background

void DrawChar(char ch, int x, int y, u32 col)
{
	int x0 = x;
	int i, j;

	// prepare pointer to font sample
	const u8* s = &Font->d[(u8)ch * 8*SCALE * 3];

	// loop through lines of one character
	for (i = 16*SCALE; i > 0; i--)
	{
		// loop through pixels of one character line
		x = x0;
		for (j = 8*SCALE; j > 0; j--)
		{
			// pixel is set
			if (*s < 128) DrawPoint(x, y, col);
			s += 3;
			x++;
		}
		s += 255*8*SCALE*3;
		y++;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Draw text, transparent background

void DrawText(const char* text, int x, int y, u32 col)
{
	u8 ch;

	// loop through characters of text
	for (;;)
	{
		// get next character of the text
		ch = (u8)*text++;
		if (ch == 0) break;

		// draw character
		DrawChar(ch, x, y, col);

		// shift to next character position
		x += 8*SCALE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Draw character with background, 8*SCALE=24 pixels width, 16*SCALE=48 pixels height

void DrawCharBg(char ch, int x, int y, u32 col, u32 bgcol)
{
	int x0 = x;
	int i, j;

	// prepare pointer to font sample
	const u8* s = &Font->d[(u8)ch * 8*SCALE * 3];

	// loop through lines of one character
	for (i = 16*SCALE; i > 0; i--)
	{
		// loop through pixels of one character line
		x = x0;
		for (j = 8*SCALE; j > 0; j--)
		{
			// pixel is set
			DrawPoint(x, y, (*s < 128) ? col : bgcol);
			s += 3;
			x++;
		}
		s += 255*8*SCALE*3;
		y++;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Draw text with background

void DrawTextBg(const char* text, int x, int y, u32 col, u32 bgcol)
{
	u8 ch;

	// loop through characters of text
	for (;;)
	{
		// get next character of the text
		ch = (u8)*text++;
		if (ch == 0) break;

		// draw character
		DrawCharBg(ch, x, y, col, bgcol);

		// shift to next character position
		x += 8*SCALE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Draw small character, 8 pixels width, 16 pixels height, transparent background

void DrawCharSmall(char ch, int x, int y, u32 col)
{
	int x0 = x;
	int i, j;

	// prepare pointer to font sample
	const u8* s = &FontSmall->d[(u8)ch * 8 * 3];

	// loop through lines of one character
	for (i = 16; i > 0; i--)
	{
		// loop through pixels of one character line
		x = x0;
		for (j = 8; j > 0; j--)
		{
			// pixel is set
			if (*s < 128) DrawPoint(x, y, col);
			s += 3;
			x++;
		}
		s += 255*8*3;
		y++;
	}

	CanDirty = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Draw small text, transparent background

void DrawTextSmall(const char* text, int x, int y, u32 col)
{
	u8 ch;

	// loop through characters of text
	for (;;)
	{
		// get next character of the text
		ch = (u8)*text++;
		if (ch == 0) break;

		// draw character
		DrawCharSmall(ch, x, y, col);

		// shift to next character position
		x += 8;
	}
}

///////////////////////////////////////////////////////////////////////////////
// shadow screen

void DispShadow()
{
	SCanvas* canvas = Canvas;
	u8* d = canvas->d;
	int i = canvas->w * canvas->h;
	u8 k;
	for (; i > 0; i--)
	{
		k = (u8)((d[0] + d[1] + d[2]) >> 3);
		d[0] = k;
		d[1] = k;
		d[2] = k;
		d += 3;
	}

	CanDirty = TRUE;
}
