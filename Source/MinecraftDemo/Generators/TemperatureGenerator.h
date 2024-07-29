#pragma once

#include "MinecraftDemo/Tools/NoiseTools.h"
#include "MinecraftDemo/Meshes/Chunk.h"

class TemperatureGenerator {
public:
	static void GenerateTemperatureField(Chunk& TheChunk);
private:
	TemperatureGenerator() = delete;
	~TemperatureGenerator() = delete;
};
