
// ****************************************************************************
//
//                                 Sounds
//
// ****************************************************************************

// WAV format
#pragma pack(push,1)

// WAV data descriptor (8 Bytes)
typedef struct WAVDATA_ {
	char			tDataIdent[4];		// (4) data identifier "data"
	DWORD			nDataSize;			// (4) size of following sound data
} WAVDATA;

// WAV format descriptor (28 Bytes)
typedef struct WAVFORMAT_ {
	char			tFormatIdent[8];	// (8) format identifier (= "WAVEfmt ")
	DWORD			nFormatSize;		// (4) size of following format data

	WORD			wFormatTag;			// (2) data format (1 = PCM)
	WORD			nChannels;			// (2) number of channels (1 = mono, 2 = stereo)
	DWORD			nSamplesPerSec;		// (4) sample frequency (number of samples per second)
	DWORD			nAvgBytesPerSec;	// (4) transfer rate (number of bytes per second)
	WORD			nBlockAlign;		// (2) data align (bits*channels/8)

	WORD			wBitsPerSample;		// (2) number of bits per one sample
} WAVFORMAT;

// WAV file header (44 B)
typedef struct WAVHEAD_ {
	char		tWavIdent[4];		// (4) file identifier (= "RIFF")
	DWORD		nFileSize;			// (4) size of following file data

	WAVFORMAT	WavFormat;			// format descriptor
	WAVDATA		WavData;			// data block
} WAVHEAD;
#pragma pack( pop )

// default sound parameters
#define WAVE_FORMAT_PCM		1
#define	SOUNDFORMAT			WAVE_FORMAT_PCM		// default wave format
#define	SOUNDCHANNELS		2					// default number of channels
#define	SOUNDRATE			44100				// default sample rate
#define	SOUNDBITS			16					// default number of bits
#define	SOUNDALIGN			(SOUNDCHANNELS*((SOUNDBITS+7)/8))	// align
#define	SOUNDBPS			(SOUNDRATE*SOUNDALIGN) // bytes per second

// sound buffers
#define SOUNDBUFFERS	4							// number of sound buffers (= 186 ms)
#define SOUNDBUFSIZE	0x800						// sound buffer size in samples (1 sample = 4 bytes; = 8192 bytes = 46 ms at 44100 Hz)
extern	HWAVEOUT		WaveOut;					// output device handle
extern	WAVEHDR			WaveHeader[SOUNDBUFFERS];	// data descriptors
extern	s16*			SoundBuf[SOUNDBUFFERS];		// sound buffers

// sound sample (format: 44100 Hz, stereo, 16-bit signed)
typedef struct {
	int		len;		// number of sound samples (1 sound sample = 2 channels * 16 bits = 32 bits
	s16		data[1];	// sound data
} sSound;

#define SOUND_SIZE sizeof(int) // sound size without data

// music sound list
#define MUSIC_NUM BG_NUM
extern sSound* MusicSnd[MUSIC_NUM];

// missile sound list
extern sSound* MissileSnd[ACT_NUM];

// misc sounds
extern sSound* Blackhole2Snd;		// 12 Devil Bird missile 2
extern sSound* EnemyHitSnd;			// enemy explosion
extern sSound* FailSnd;				// fail hit
extern sSound* ZingSnd;				// get bonus
extern sSound* BigbonusSnd;			// big bonus

// sound channels
typedef struct {
	sSound*		snd;		// sound (NULL = channel not used)
	int			off;		// playing offset
	BOOL		rep;		// repeat
	int			vol;		// relative volume (1024 = normal)
} sChannel;

#define SOUNDCHAN_MISSILE	0	// sound channel - missiles thrown by actor
#define SOUNDCHAN_ENEMYHIT	1	// sound channel - enemy hit
#define SOUNDCHAN_ACTORHIT	2	// sound channel - actor hit
#define SOUNDCHAN_MUSIC		3	// sound channel - music

#define SOUNDCHAN_NUM		4	// number of sound channels

extern sChannel SndChannel[SOUNDCHAN_NUM]; // sound channels

// sound mode
#define SOUNDMODE_OFF	0	// sound is OFF
#define SOUNDMODE_SOUND	1	// only sound
#define SOUNDMODE_MUSIC	2	// sound + music

extern int SoundMode; // sound mode

// load one sound
BOOL LoadSnd(sSound** snd, u16 id);

// load all sounds
BOOL LoadAllSnd();

// shift sound channels
void ShiftSound();

// play sound (vol = relative volume)
void PlaySound(int chan, sSound* snd, BOOL rep, float vol);

// play music
void MusicStart();

// stop sounds
void StopAllSound();
