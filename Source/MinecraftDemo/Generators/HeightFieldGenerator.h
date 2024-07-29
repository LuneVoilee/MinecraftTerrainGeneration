#pragma once
#include "../Meshes/Chunk.h"
#include "../Tools/NoiseTools.h"
#include <iostream>

class HeightFieldGenerator {
public:
	
	static void GeneratorHeightField(Chunk& TheChunk);
	HeightFieldGenerator()=delete;
	~HeightFieldGenerator()=delete;
};
