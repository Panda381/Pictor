
// ****************************************************************************
//
//                                 Sounds
//
// ****************************************************************************

#include "Include.h"

HWAVEOUT	WaveOut = NULL;					// output device handle
WAVEHDR		WaveHeader[SOUNDBUFFERS];		// data descriptors
s16*		SoundBuf[SOUNDBUFFERS];			// sound buffers

// music sound list
sSound* MusicSnd[MUSIC_NUM];

// missile sound list
sSound* MissileSnd[ACT_NUM];

// misc sounds
sSound* Blackhole2Snd;		// 12 Devil Bird missile 2
sSound* EnemyHitSnd;		// enemy explosion
sSound* FailSnd;			// fail hit
sSound* ZingSnd;			// get bonus
sSound* BigbonusSnd;		// big bonus

sChannel SndChannel[SOUNDCHAN_NUM]; // sound channels

int SoundMode = DEFSOUNDMODE; // sound mode

// load one sound
BOOL LoadSnd(sSound** snd, u16 id)
{
	// find resource
	HRSRC res = ::FindResource(Instance, MAKEINTRESOURCE(id), "WAV");
	if (res == NULL) return FALSE;

	// lock resource
	HGLOBAL glob = ::LoadResource(Instance, res);
	if (glob == NULL) return FALSE;

	// get image address
	u8* a = (u8*)::LockResource(glob);
	if (a == NULL) return FALSE;

	// check WAV header
	WAVHEAD* wav = (WAVHEAD*)a;
	WAVFORMAT* fmt = &wav->WavFormat;
	WAVDATA* data = &wav->WavData;
	if ((memcmp(wav->tWavIdent, "RIFF", 4) != 0) || // check "RIFF" header
		(memcmp(fmt->tFormatIdent, "WAVEfmt ", 8) != 0) || // check "WAVEfmt " header
		(memcmp(data->tDataIdent, "data", 4) != 0) || // check "data" header
		(fmt->wFormatTag != SOUNDFORMAT) || // check PCM format
		(fmt->nChannels != SOUNDCHANNELS) || // check channels
		(fmt->nSamplesPerSec != SOUNDRATE) || // check rate
		(fmt->wBitsPerSample != SOUNDBITS)) // check bits per sample
	{
		return FALSE;
	}
	u8* src = (u8*)&data[1];			// start of sound data
	int n = data->nDataSize;			// size of sound data in bytes

	// load sound data
	sSound* s = (sSound*)malloc(n + SOUND_SIZE + 10);
	if (s == NULL) return FALSE;
	memcpy(s->data, src, n);
	s->len = n / SOUNDALIGN; // number of samples
	*snd = s;

	return TRUE;
}

// load all sounds
BOOL LoadAllSnd()
{
	// music sound
	if (!LoadSnd(&MusicSnd[0], WAV_MUSIC1)) return FALSE;
	if (!LoadSnd(&MusicSnd[1], WAV_MUSIC2)) return FALSE;
	if (!LoadSnd(&MusicSnd[2], WAV_MUSIC3)) return FALSE;
	if (!LoadSnd(&MusicSnd[3], WAV_MUSIC4)) return FALSE;
	if (!LoadSnd(&MusicSnd[4], WAV_MUSIC5)) return FALSE;
	if (!LoadSnd(&MusicSnd[5], WAV_MUSIC6)) return FALSE;
	if (!LoadSnd(&MusicSnd[6], WAV_MUSIC7)) return FALSE;
	if (!LoadSnd(&MusicSnd[7], WAV_MUSIC8)) return FALSE;
	if (!LoadSnd(&MusicSnd[8], WAV_MUSIC9)) return FALSE;
	if (!LoadSnd(&MusicSnd[9], WAV_MUSIC10)) return FALSE;
	if (!LoadSnd(&MusicSnd[10], WAV_MUSIC11)) return FALSE;
	if (!LoadSnd(&MusicSnd[11], WAV_MUSIC12)) return FALSE;

	// missile sound
	if (!LoadSnd(&MissileSnd[0], WAV_SWING)) return FALSE;
	if (!LoadSnd(&MissileSnd[1], WAV_MINIGUN)) return FALSE;
	if (!LoadSnd(&MissileSnd[2], WAV_GUN)) return FALSE;
	if (!LoadSnd(&MissileSnd[3], WAV_NOISE)) return FALSE;
	if (!LoadSnd(&MissileSnd[4], WAV_ELECTRICITY)) return FALSE;
	if (!LoadSnd(&MissileSnd[5], WAV_GLASS)) return FALSE;
	if (!LoadSnd(&MissileSnd[6], WAV_FLAME)) return FALSE;
	if (!LoadSnd(&MissileSnd[7], WAV_SCREAMING)) return FALSE;
	if (!LoadSnd(&MissileSnd[8], WAV_LASERSHOT)) return FALSE;
	if (!LoadSnd(&MissileSnd[9], WAV_PHASERSHOT)) return FALSE;
	if (!LoadSnd(&MissileSnd[10], WAV_ROCKETSHOT)) return FALSE;
	if (!LoadSnd(&MissileSnd[11], WAV_THROW)) return FALSE;
	if (!LoadSnd(&MissileSnd[12], WAV_BLACKHOLE)) return FALSE;

	// misc
	if (!LoadSnd(&Blackhole2Snd, WAV_BLACKHOLE2)) return FALSE;
	if (!LoadSnd(&EnemyHitSnd, WAV_ENEMYHIT)) return FALSE;
	if (!LoadSnd(&FailSnd, WAV_FAIL)) return FALSE;
	if (!LoadSnd(&ZingSnd, WAV_ZING)) return FALSE;
	if (!LoadSnd(&BigbonusSnd, WAV_BIGBONUS)) return FALSE;

	// initialize sound buffers
	int i;
	for (i = 0; i < SOUNDBUFFERS; i++)
	{
		WaveHeader[i].dwFlags = WHDR_DONE;
		SoundBuf[i] = (s16*)malloc(SOUNDBUFSIZE*SOUNDALIGN);
		if (SoundBuf[i] == NULL) return FALSE;
	}

	// prepare format descriptor
	WAVEFORMATEX wf;
	wf.wFormatTag = SOUNDFORMAT;
	wf.nChannels = SOUNDCHANNELS;
	wf.nSamplesPerSec = SOUNDRATE;
	wf.nAvgBytesPerSec = SOUNDBPS;
	wf.nBlockAlign = SOUNDALIGN;
	wf.wBitsPerSample = SOUNDBITS;
	wf.cbSize = 0;

	// open output device
	MMRESULT res = ::waveOutOpen(&WaveOut, WAVE_MAPPER, &wf, NULL, NULL, CALLBACK_NULL);
	if (res != MMSYSERR_NOERROR) return FALSE;

	// initialize sound channels
	for (i = 0; i < SOUNDCHAN_NUM; i++) SndChannel[i].snd = NULL;

	return TRUE;
}

// shift sound channels
void ShiftSound()
{
	// find next free sound buffer
	int i;
	WAVEHDR* wh = WaveHeader;
	for (i = 0; i < SOUNDBUFFERS; i++)
	{
		// check if buffer is free
		if (wh->dwFlags & WHDR_DONE)
		{
			// unprepare buffer
			if (wh->dwFlags & WHDR_PREPARED)
				::waveOutUnprepareHeader(WaveOut, wh, sizeof(WAVEHDR));

			// clear buffer
			s16* d = SoundBuf[i];
			memset(d, 0, SOUNDBUFSIZE*SOUNDALIGN);

			// render sound channels
			int j;
			for (j = 0; j < SOUNDCHAN_NUM; j++)
			{
				sChannel* ch = &SndChannel[j];

				// check if channel is used
				sSound* s = ch->snd;
				if (s != NULL)
				{
					// relative volume
					int vol = ch->vol;

					// position in buffer
					int pos = 0;

					// while there is an empty space in the buffer
					while (pos < SOUNDBUFSIZE*SOUNDCHANNELS)
					{
						// remaining space in buffer
						int rem = SOUNDBUFSIZE*SOUNDCHANNELS - pos;

						// number of sound samples
						int len = s->len * SOUNDCHANNELS;

						// offset in source sound
						int off = ch->off;

						// number of remaining samples
						len -= off;
						if (len > rem) len = rem;

						// add samples
						while (len > 0)
						{
							int b = d[pos] + ((s->data[off] * vol) >> 10);
							if (b < -32767) b = -32767;
							if (b > 32767) b = 32767;
							d[pos] = (s16)b;
							pos++;
							off++;
							len--;
						}
						ch->off = off;

						// end of sound
						len = s->len * SOUNDCHANNELS;
						if (off >= len)
						{
							// repeat
							if (ch->rep)
								ch->off = 0;
							else
							{
								ch->snd = NULL;
								break;
							}
						}
					}
				}
			}

			// prepare header
			wh->lpData = (char*)d;
			wh->dwBufferLength = SOUNDBUFSIZE*SOUNDALIGN;
			wh->dwBytesRecorded = SOUNDBUFSIZE*SOUNDALIGN;
			wh->dwUser = 0;
			wh->dwFlags = 0;
			wh->dwLoops = 0;
			wh->lpNext = NULL;
			wh->reserved = 0;
			waveOutPrepareHeader(WaveOut, wh, sizeof(WAVEHDR));

			// send buffer
			waveOutWrite(WaveOut, wh, sizeof(WAVEHDR));
		}

		wh++;
	}
}

// play sound (vol = relative volume)
void PlaySound(int chan, sSound* snd, BOOL rep, float vol)
{
	sChannel* ch = &SndChannel[chan];
	ch->snd = snd;
	ch->off = 0;
	ch->rep = rep;
	ch->vol = (int)(vol*1024 + 0.5f);
}

// play music
void MusicStart()
{
	if (SoundMode == SOUNDMODE_MUSIC)
		PlaySound(SOUNDCHAN_MUSIC, MusicSnd[BackInx], True, SOUNDVOL_MUSIC);
}

// stop sounds
void StopAllSound()
{
	int i;
	for (i = 0; i < SOUNDCHAN_NUM; i++) SndChannel[i].snd = NULL;
}

