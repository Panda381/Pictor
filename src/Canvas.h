
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                 Canvas Drawing                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

typedef struct SCanvas_ {
	u16		w;		// canvas width
	u16		h;		// canvas height
	u16		res;	// ... reserved (align)
//	u16		ref;	// share reference counter of owners (single-threaded!, 0=no owner, can be deleted)
	u8		c;		// color bytes (3 if BGR, 4 if BGRA with alpha)
	u8		res2;	// ... reserved (align)
	u8		d[1];	// canvas data (in BGR or BGRA format) + 1 byte reserve to enable u32 operations
} SCanvas;

#define CANVAS_SIZE (3*sizeof(u16) + 2*sizeof(u8)) // canvas size without data

extern SCanvas* Canvas;		// draw canvas
extern SCanvas* Font;		// font 24x48
extern SCanvas* FontSmall;	// small font 8x16

///////////////////////////////////////////////////////////////////////////////
// color as DWORD (a=0 transparency .. 255 opaque)

//#define RGBACOL(r,g,b,a) ((DWORD)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)))
//#define RGBCOL(r,g,b) RGBACOL(r,g,b,255)

//#define RGB_R(rgb) (((rgb) >> 16) & 0xff)
//#define RGB_G(rgb) (((rgb) >> 8) & 0xff)
//#define RGB_B(rgb) ((rgb) & 0xff)
//#define RGB_A(rgb) (((rgb) >> 24) & 0xff)

// color as QUADRUPLE (palette entry)
/*typedef struct SRGBA_ {
		BYTE	b;
		BYTE	g;
		BYTE	r;
		BYTE	a;
} SRGBA;
*/

#define COLOR(r,g,b) ((DWORD)( (((r)&0xff)<<16) | (((g)&0xff)<<8) | ((b)&0xff) ))

// - base colors
#define COL_BLACK		COLOR(  0,  0,  0)
#define COL_BLUE		COLOR(  0,  0,255)
#define COL_GREEN		COLOR(  0,255,  0)
#define COL_CYAN		COLOR(  0,255,255)
#define COL_RED			COLOR(255,  0,  0)
#define COL_MAGENTA		COLOR(255,  0,255)
#define COL_YELLOW		COLOR(255,255,  0)
#define COL_WHITE		COLOR(255,255,255)
#define COL_GRAY		COLOR(128,128,128)
// - dark colors
#define COL_DKBLUE		COLOR(  0,  0,127)
#define COL_DKGREEN		COLOR(  0,127,  0)
#define COL_DKCYAN		COLOR(  0,127,127)
#define COL_DKRED		COLOR(127,  0,  0)
#define COL_DKMAGENTA	COLOR(127,  0,127)
#define COL_DKYELLOW	COLOR(127,127,  0)
#define COL_DKWHITE		COLOR(127,127,127)
#define COL_DKGRAY		COLOR( 64, 64, 64)
// - light colors
#define COL_LTBLUE		COLOR(127,127,255)
#define COL_LTGREEN		COLOR(127,255,127)
#define COL_LTCYAN		COLOR(127,255,255)
#define COL_LTRED		COLOR(255,127,127)
#define COL_LTMAGENTA	COLOR(255,127,255)
#define COL_LTYELLOW	COLOR(255,255,127)
#define COL_LTGRAY		COLOR(192,192,192)

#define COL_AZURE		COLOR(  0,127,255)
#define COL_ORANGE		COLOR(255,127,  0)

///////////////////////////////////////////////////////////////////////////////
// Draw point

void DrawPoint(int x, int y, u32 col);

///////////////////////////////////////////////////////////////////////////////
// create canvas using malloc (c = 3 or 4; ref=1; returns NULL on error)

inline SCanvas* NewCanvas(int w, int h, int c = 3)
{
	SCanvas* b = (SCanvas*)malloc(CANVAS_SIZE + w*h*c + 10);
	if (b != NULL)
	{
		b->w = (u16)w;
		b->h = (u16)h;
//		b->ref = 1;
		b->c = (u8)c;
	}
	return b;
}

///////////////////////////////////////////////////////////////////////////////
// reallocate canvas using realloc (c = 3 or 4; returns NULL on error; ref not changed)
/*
inline SCanvas* ReallocCanvas(SCanvas* old, int w, int h, int c = 3)
{
	SCanvas* b = (SCanvas*)realloc(old, CANVAS_SIZE + w*h*c + 10);
	if (b != NULL)
	{
		b->w = (u16)w;
		b->h = (u16)h;
		b->c = (u8)c;
	}
	return b;
}
*/
///////////////////////////////////////////////////////////////////////////////
// attach canvas to owner
/*
inline void AttachCanvas(SCanvas* c)
{
	c->ref++;
}
*/
///////////////////////////////////////////////////////////////////////////////
// dettach canvas from owner and delete if not needed
/*
inline void DetachCanvas(SCanvas* c)
{
	c->ref--;
	if (c->ref == 0) free(c);
}
*/
///////////////////////////////////////////////////////////////////////////////
// flip RGB and BGR

//void RGBflipBGR(SCanvas* c);

///////////////////////////////////////////////////////////////////////////////
// vertical flip canvas

void VFlipCanvas(SCanvas* c);

///////////////////////////////////////////////////////////////////////////////
// draw horizontal line

void DrawHLine(int x, int y, int w, u32 col);

///////////////////////////////////////////////////////////////////////////////
// draw vertical line

void DrawVLine(int x, int y, int h, u32 col);

///////////////////////////////////////////////////////////////////////////////
// draw frame

void DrawFrame(int x, int y, int w, int h, int thick, u32 col);

///////////////////////////////////////////////////////////////////////////////
// draw horizontal dashed line

//void DrawHDash(SCanvas* dst, int x, int y, int w, u32 col1, u32 col2);

///////////////////////////////////////////////////////////////////////////////
// draw vertical dashed line

//void DrawVDash(SCanvas* dst, int x, int y, int h, u32 col1, u32 col2);

///////////////////////////////////////////////////////////////////////////////
// draw dashed frame

//void DrawDash(SCanvas* dst, int x, int y, int w, int h, u32 col1, u32 col2);

///////////////////////////////////////////////////////////////////////////////
// draw rectangle

void DrawRect(int x, int y, int w, int h, u32 col);

///////////////////////////////////////////////////////////////////////////////
// clear screen

void DrawClear();

///////////////////////////////////////////////////////////////////////////////
// draw arrow

//void DrawArrow(SCanvas* dst, int x, int y, u32 col);

///////////////////////////////////////////////////////////////////////////////
// draw image

//void DrawImage(SCanvas* dst, SCanvas* src, int x, int y, int h = 0);

///////////////////////////////////////////////////////////////////////////////
// draw image cut

//void DrawImageCut(SCanvas* dst, SCanvas* src, int dx, int dy, int sx, int sy, int w, int h);

///////////////////////////////////////////////////////////////////////////////
// Draw image (source and destination must have same format)

void DrawImage(SCanvas* src, int xs, int ys, int xd, int yd, int w, int h);

///////////////////////////////////////////////////////////////////////////////
// Draw image transparent

void DrawImageTrans(SCanvas* src, int xs, int ys, int xd, int yd, int w, int h);

///////////////////////////////////////////////////////////////////////////////
// Draw image shadow

void DrawImgShadow(SCanvas* src, int xd, int yd);

///////////////////////////////////////////////////////////////////////////////
// draw image with transparency

//void DrawImageTrans(SCanvas* dst, SCanvas* src, int dx, int dy, int sx, int sy, int w, int h, u32 transcol);

///////////////////////////////////////////////////////////////////////////////
// draw line

//void DrawLine(SCanvas* dst, int x1, int y1, int x2, int y2, u32 col);

///////////////////////////////////////////////////////////////////////////////
// decode picture from memory (returns NULL on error; return Canvas with ref=1)

//SCanvas* ImgDecode(const u8* buf, int size);

///////////////////////////////////////////////////////////////////////////////
// Draw character, 8*SCALE=24 pixels width, 16*SCALE=48 pixels height, transparent background

void DrawChar(char ch, int x, int y, u32 col);

///////////////////////////////////////////////////////////////////////////////
// Draw text, transparent background

void DrawText(const char* text, int x, int y, u32 col);

///////////////////////////////////////////////////////////////////////////////
// Draw character with background, 8*SCALE=24 pixels width, 16*SCALE=48 pixels height

void DrawCharBg(char ch, int x, int y, u32 col, u32 bgcol);

///////////////////////////////////////////////////////////////////////////////
// Draw text with background

void DrawTextBg(const char* text, int x, int y, u32 col, u32 bgcol);

///////////////////////////////////////////////////////////////////////////////
// Draw small character, 8 pixels width, 16 pixels height, transparent background

void DrawCharSmall(char ch, int x, int y, u32 col);

///////////////////////////////////////////////////////////////////////////////
// Draw small text, transparent background

void DrawTextSmall(const char* text, int x, int y, u32 col);

///////////////////////////////////////////////////////////////////////////////
// shadow screen

void DispShadow();
