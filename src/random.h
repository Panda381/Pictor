
// ****************************************************************************
//
//                            Random number generator
//
// ****************************************************************************
// - used for defined reproduction of enemies in the scene

// Seed of random number generator to generate enemies
extern u32 EnemyRandSeed;
extern u64 RandSeed;

// generate 16-bit unsigned integer random number
u16 EnemyRand();

// generate 16-bit unsigned integer random number in range 0 to MAX (including)
u16 EnemyRandMax(u16 max);

// generate 16-bit unsigned integer random number in range MIN to MAX (including)
// If MIN > MAX, then number is generated out of interval.
u16 EnemyRandMinMax(u16 min, u16 max);

// generate 16-bit signed integer random number in range MIN to MAX (including)
// If MIN > MAX, then number is generated out of interval.
s16 EnemyRandSMinMax(s16 min, s16 max);

// random number generator
u32 RandU32();
u8 RandU8();
u16 RandU16();
u16 RandU16Max(u16 max);
s16 RandS16Max(s16 max);
u16 RandU16MinMax(u16 min, u16 max);
s16 RandS16MinMax(s16 min, s16 max);
