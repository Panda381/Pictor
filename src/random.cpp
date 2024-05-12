
// ****************************************************************************
//
//                            Random number generator
//
// ****************************************************************************
// - used for defined reproduction of enemies in the scene

#include "Include.h"

// Seed of random number generator to generate enemies
u32 EnemyRandSeed;
u64 RandSeed;

// generate 16-bit unsigned integer random number
u16 EnemyRand()
{
	u32 s = EnemyRandSeed*214013 + 2531011;
	EnemyRandSeed = s;
	return (u16)(s >> 16);
}

// generate 16-bit unsigned integer random number in range 0 to MAX (including)
u16 EnemyRandMax(u16 max)
{
	u16 res;
	u16 msk;

	// zero maximal value
	if (max == 0) return 0;

	// prepare mask
	msk = (u16)Mask(max);

	// generate random number
	do {
		res = EnemyRand() & msk;
	} while (res > max);

	return res;
}

// generate 16-bit unsigned integer random number in range MIN to MAX (including)
// If MIN > MAX, then number is generated out of interval.
u16 EnemyRandMinMax(u16 min, u16 max)
{
	return EnemyRandMax(max - min) + min;
}

// generate 16-bit signed integer random number in range MIN to MAX (including)
// If MIN > MAX, then number is generated out of interval.
s16 EnemyRandSMinMax(s16 min, s16 max)
{
	return (s16)EnemyRandMax(max - min) + min;
}

// random number generator
u32 RandU32()
{
	u64 k = RandSeed;
	k = k*214013 + 2531011;
	RandSeed = k;
	return (u32)(k >> 32);
}

u8 RandU8()
{
	return (u8)(RandU32() >> 24);
}

u16 RandU16()
{
	return (u16)(RandU32() >> 16);
}

u16 RandU16Max(u16 max)
{
	u16 res;
	u16 msk;

	// zero maximal value
	if (max == 0) return 0;

	// prepare mask
	msk = (u16)Mask(max);

	// generate random number
	do {
		res = RandU16() & msk;
	} while (res > max);

	return res;
}

s16 RandS16Max(s16 max)
{
	if (max >= 0)
		return (s16)RandU16Max((u16)max);
	else
		return -(s16)RandU16Max((u16)-max);
}

u16 RandU16MinMax(u16 min, u16 max)
{
	return RandU16Max(max - min) + min;
}

s16 RandS16MinMax(s16 min, s16 max)
{
	return (s16)RandU16Max(max - min) + min;
}
