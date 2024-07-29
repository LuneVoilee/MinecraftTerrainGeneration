#pragma once
#include "MinecraftDemo/Meshes/Chunk.h"

class BiomeGenerator {
public:
	static void GenerateBiomeField(Chunk& TheChunk);
private:
	BiomeGenerator()=delete;
	~BiomeGenerator()=delete;
};

enum Biome{
	None = 0,
	Snow = 1,
	Green = 2,
	Dry = 3,
	Stone = 4,
	Desert = 5
};