#pragma once
#include "../Meshes/Chunk.h"
#include "../Tools/NoiseTools.h"
#include <iostream>
#include <unordered_map>

class HeightFieldGenerator {
public:
	
	static void GeneratorHeightField(Chunk& TheChunk, TMap<uint64, int32>& HeightPool);
	HeightFieldGenerator()=delete;
	~HeightFieldGenerator()=delete;
};
