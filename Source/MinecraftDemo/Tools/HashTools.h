#pragma once

class HashTools {
	HashTools()=delete;
	~HashTools()=delete;
public:
	static inline uint64_t seed = 0x24fab946dc3175;
	static uint64 Vec3HashToUint64(int x,int y,int z=0);
	static FVector Uint64UnhashToVec3(uint64 pos);
	static int32 Int32HashToInt32(int32 t);
	static FVector2d Vec2HashToVec2(FVector2d vec);
	static double DeterministicRandom(int64_t begin,int64_t end);
	static double NonDeterministicRandom(int64_t begin, int64_t end);
	static void SetSeed(uint64_t NewSeed);
};
