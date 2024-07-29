#include "HashTools.h"

#include <random>

uint64 HashTools::Vec3HashToUint64(int32 x, int32 y, int32 z) {
	int32 power_14=16384;
	uint64 res= uint64(x+power_14)<<40|
				uint64(y+power_14)<<20|
				uint64(z+power_14);
	return res;
};

FVector HashTools::Uint64UnhashToVec3(uint64 pos) {
 	int32 power_14 = 16384;
 	float x = static_cast<float>(int32(pos >> 40) - power_14) ;
 	float y = static_cast<float>(int32((pos >> 20) & 0xFFFFF) - power_14);
 	float z = static_cast<float>(int32(pos & 0xFFFFF) - power_14);
 	return {x, y, z};
}



int32 HashTools::Int32HashToInt32(int32 t) {
	const uint32 BIT_NOISE1 = 0x85297A4D;
	const uint32 BIT_NOISE2 = 0x68E31DA4;
	const uint32 BIT_NOISE3 = 0x1B56C4E9;
	uint32 mangled = t;
	mangled *= BIT_NOISE1;
	mangled ^= (mangled >> 8);
	mangled += BIT_NOISE2;
	mangled ^= (mangled << 8);
	mangled *= BIT_NOISE3;
	mangled ^= (mangled >> 8);
	return mangled%1024;
}

FVector2d HashTools::Vec2HashToVec2(FVector2d vec) {
	FVector2D v(
		Int32HashToInt32(0x651A6BE3 * (int32)vec.X - (int32)vec.Y)%1024,
		Int32HashToInt32((int32)vec.X * (int32)vec.Y + 0x218AE247)%1024
		);
	v/=1024.0f;
	return v;
}

double HashTools::DeterministicRandom(int64_t begin,int64_t end) {
	std::mt19937 rand(seed);
	std::uniform_real_distribution<double> dist(begin, end);
	return dist(rand);
}
double HashTools::NonDeterministicRandom(int64_t begin,int64_t end) {
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_real_distribution<double> dist(begin, end);
	return dist(rand);
}

void HashTools::SetSeed(uint64_t NewSeed) {
	HashTools::seed = NewSeed;
}
