
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                   Includes                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// program setup

/* Levels: background ... actor ... attack ... enemy1, enemy2, enemy3
 0                  Jill .............. Shurigen
 1 Meadow ......... Bird .............. Seed ................ Fly, Wasp, Hornet
 2 Jungle ......... Parrot ............ Berry ............... Dragonfly, Moth, Kestrel
 3 Sandy Beach .... Scarabeus ......... Sand Ball ........... Maggot, Eagle, Antman
 4 Underwater ..... Stingray .......... Lightning ........... Fish, Anglerfish, Shark
 5 Ice Land ....... Ice Bird .......... Ice Crystal ......... Penguin, Polar Bear, Snowflake
 6 Fiery Land ..... Dragon ............ Fire Ball ........... Phoenix, Fireball, Evil
 7 Haunted Hill ... Ghost ............. Screaming ........... Pumpkin, Zombie, Death
 8 Spacecraft ..... R2-D2 ............. Laser ............... Camera, Explorer, War Dron
 9 Galaxy ......... UFO ............... Phaser .............. Starship Enterprise, Klingon Warship, Death Star
10 Alien Planet ... Rocket Squirrel ... Rocket .............. Alien, Xenomorph, Meteor
11 Candy Land ..... Gingerbread Man ... Sugar Bomb .......... Candy Cane, Lollipop, Fat Man
12 Surreal Land ... Devil Bird ........ Hole of Nothingness . Cloud, Star, Moon
*/

#define VERSION		"Pictor v1.0, FREEWARE, (c) 2024 Miroslav Nemecek, www.breatharian.eu/Panda38"

// Debug
#define DEBUG		0		// 1=debug mode

#define SCALE		3		// graphics scale
#define SUBFRAMES	SCALE	// sub-frames
#define TIMERTIME	30		// time of main timer (in [ms])

#if DEBUG		// 1=debug mode

// debug mode
#define STARTLEVEL		0					// start level-1 (default 0 = Sector 1)
#define DEB_ENDLESS		0					// 1=endless level
#define DEFSOUNDMODE	SOUNDMODE_MUSIC		// initial sound mode 0=off, 1=sound, 2=all
#define DEB_ALLACTORS	0					// 1=all actors are available, 0=normal mode
#define DEB_CUSTOMLEVEL	0					// 1=select level using Home/End keys
#define DEB_IMMORTALITY	0					// 1=immortality
#define DEB_ALLSHOOT	0					// 1=shoot all the time

#else

// normal mode (do not change!)
#define STARTLEVEL		0					// start level-1 (default 0 = Sector 1)
#define DEB_ENDLESS		0					// 1=endless level
#define DEFSOUNDMODE	SOUNDMODE_MUSIC		// initial sound mode 0=off, 1=sound, 2=all
#define DEB_ALLACTORS	0					// 1=all actors are available, 0=normal mode
#define DEB_CUSTOMLEVEL	0					// 1=select level using Home/End keys
#define DEB_IMMORTALITY	0					// 1=immortality
#define DEB_ALLSHOOT	0					// 1=shoot all the time

#endif

// sound volumes
#define SOUNDVOL_MISSILE	1.0f		// missile volume
#define SOUNDVOL_ENEMYHIT	1.0f		// enemy hit volume
#define SOUNDVOL_SUCK		1.2f		// black hole suck volume
#define SOUNDVOL_ACTORHIT	1.2f		// actor hit volume
#define SOUNDVOL_BONUS		1.0f		// bonus collect volume
#define SOUNDVOL_MUSIC		0.6f		// music volume

#pragma comment(lib, "Winmm.lib")		// sound

#define TOPLIST_NAME	"Pictor.top"	// top list filename

// Game difficulty settings
#define ENEMY_GEN_LEVEL	1.06f	// increase number of enemies per level (relative float number)
#define GAMELEN		700		// game length in number of frames (equals to approx. 1 minute)

// One level settings
#define ENEMY_GEN_BEG	8000	// probability of generating enemies at the beginning of the scene (number 0..65535)
#define ENEMY_GEN_END	14000	// probability of generating enemies at the end of the scene (number 0..65535)
#define ENEMY_GEN_BUL	1200	// probability of generating enemy bullet (number 0..65536)

#define MUSIC_VOL	0.4f	// music volume (float number, 1.0f = max.)

// main setup
#define BG_NUM		12		// number of backgrounds
#define ACT_NUM		(BG_NUM+1)	// number of actors, including Jill (= 13)
#define ENEMY_LEVNUM	3		// number of enemies per level
#define ENEMY_NUM	(BG_NUM*ENEMY_LEVNUM) // number of enemy templates (= 36)
#define ENEMY_MAX	20		// max. enemies on the screen

#define WIDTH		(320*SCALE)	// screen width (= 960)
#define HEIGHT		(240*SCALE)	// screen height (= 720)

///////////////////////////////////////////////////////////////////////////////
// compilation switches
// _DEBUG	... debug version

#pragma warning ( disable: 4100)		// warning - unreferenced parameter
#pragma warning ( disable: 4310)		// warning - cast truncates constant value
#pragma warning ( disable: 4996)		// warning - unsafe function

///////////////////////////////////////////////////////////////////////////////
// system includes

#include <windows.h>
#include <tchar.h>
#define WM_MOUSEWHEEL          0x020A
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PICMAXWH	16384		// picture maximal width and height

///////////////////////////////////////////////////////////////////////////////
// debug

#ifdef _DEBUG
#define ASSERT(f)	{ if (!(f)) _asm{int 3}; }
#else
#define ASSERT(f)	((void)0)
#endif	// _DEBUG

///////////////////////////////////////////////////////////////////////////////
// base types

typedef __int8				s8;
typedef __int16				s16;
typedef __int32				s32;
typedef __int64				s64;

typedef unsigned __int8		u8;
typedef unsigned __int16	u16;
typedef unsigned __int32	u32;
typedef unsigned __int64	u64;

typedef unsigned char Bool;
#define True 1
#define False 0

///////////////////////////////////////////////////////////////////////////////
// constants

// bits
#define	B0	0x1
#define	B1	0x2
#define	B2	0x4
#define	B3	0x8
#define	B4	0x10
#define	B5	0x20
#define	B6	0x40
#define	B7	0x80
#define	B8	0x100
#define	B9	0x200
#define	B10	0x400
#define	B11	0x800
#define	B12	0x1000
#define	B13	0x2000
#define	B14	0x4000
#define	B15	0x8000
#define B16 0x10000
#define B17	0x20000
#define B18	0x40000
#define	B19	0x80000
#define B20 0x100000
#define B21 0x200000
#define B22 0x400000
#define B23 0x800000
#define B24 0x1000000
#define B25 0x2000000
#define B26 0x4000000
#define B27 0x8000000
#define B28 0x10000000
#define B29 0x20000000
#define B30 0x40000000
#define B31 0x80000000

#define BIT(pos) (1UL<<(pos))

#define	BIGINT	0x40000000

///////////////////////////////////////////////////////////////////////////////
// inplace "new" operator
//		example:	new (&m_List[inx]) CText;
//					m_List[i].~CText()

inline void* operator new (size_t size, void* p)
{
	size;
	return p;
}

inline void operator delete (void* adr, void* p)
{
	adr; p;
	return;
}

// count of array entries
#define count_of(a) (sizeof(a)/sizeof((a)[0]))

// get number of leading zeros of u32 number
// takes 296 ns ... better to use bootrom version clz(), it takes 161 ns

inline u8 clz(u32 val)
{
	u8 i = 32;

	if (val >= 0x10000)
	{
		i = 16;
		val >>= 16;
	}

	if (val >= 0x100)
	{
		i -= 8;
		val >>= 8;
	}

	if (val >= 0x10)
	{
		i -= 4;
		val >>= 4;
	}

	if (val >= 4)
	{
		i -= 2;
		val >>= 2;
	}

	if (val >= 2)
	{
		i -= 1;
		val >>= 1;
	}

	if (val >= 1) i--;

	return i;
}

// get mask of value (0x123 returns 0x1FF)
inline u32 Mask(u32 val) { return (u32)-1 >> clz(val); }

///////////////////////////////////////////////////////////////////////////////
// program includes

#include "resource.h"

// Canvas
#include "Canvas.h"				// canvas
#include "tga.h"				// TGA file

#include "header.h"				// game header
#include "background.h"			// background
#include "foot.h"				// game foot

#include "actor.h"				// actor
#include "bonus.h"				// bonus
#include "bullet.h"				// enemy bullets
#include "enemy.h"				// enemies
#include "game.h"				// game
#include "gamemenu.h"			// game menu
#include "gameend.h"			// game end
#include "gamestart.h"			// game start
#include "missile.h"			// actor's missile
#include "point.h"				// points
#include "random.h"				// random number generator
#include "titlescreen.h"		// title screen

#include "sound.h"				// sound and music
#include "MainFrame.h"			// application window
#include "Main.h"				// main module
