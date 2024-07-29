#pragma once
#include "MinecraftDemo/Meshes/Chunk.h"
#include "MinecraftDemo/Tools/HashTools.h"
#include "MinecraftDemo/Tools/NoiseTools.h"

class HumidityGenerator {
public:
	static void GenerateHumidityField(Chunk& TheChunk);
	
private:
	HumidityGenerator() = delete;
	~HumidityGenerator() = delete;
};

