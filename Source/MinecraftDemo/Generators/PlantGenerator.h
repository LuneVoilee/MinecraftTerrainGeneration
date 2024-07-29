#pragma once
#include "MinecraftDemo/Meshes/Chunk.h"
#include "MinecraftDemo/Core/MinecraftGameMode.h"
DECLARE_LOG_CATEGORY_EXTERN(LOG_PLANT, Log, All);

class PlantGenerator {
public:
	static void GeneratorPlant(Chunk& TheChunk,TMap<uint64,int32>& CubeTypePool,TMap<uint64,int32>& CubeReadyToDisplay);
	static bool GeneratorTree(Chunk& TheChunk,TMap<uint64,int32>& CubeTypePool,TMap<uint64,int32>& CubeReadyToDisplay,int32 i,int32 j);
	static bool GeneratorFlower(Chunk& TheChunk,TMap<uint64,int32>& CubeTypePool,TMap<uint64,int32>& CubeReadyToDisplay,int32 i,int32 j);
	static int32 getRandInt(int32 a , int32 b);
	static void DisplayPlant(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay,int32 i,int32 j,int32 k,int32 Type);
	static void GenerateLeaves(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay,int32 x,int32 y,int32 height,int32 radius,int32 LeavesType);

	static inline bool leavesTemplate[4][5][5] = {
			//一圈树叶
		{ {0,0,0,0,0},
			{0,0,1,0,0},
			{0,1,1,1,0},
			{0,0,1,0,0},
			{0,0,0,0,0}},
			//一圈半树叶
		{ {0,0,0,0,0},
			{0,1,1,1,0},
			{0,1,1,1,0},
			{0,1,1,1,0},
			{0,0,0,0,0}},
			//一圈半树叶
		{ {0,1,1,1,0},
			{1,1,1,1,1},
			{1,1,1,1,1},
			{1,1,1,1,1},
			{0,1,1,1,0}},
			//两圈树叶
		{ {1,1,1,1,1},
			{1,1,1,1,1},
			{1,1,1,1,1},
			{1,1,1,1,1},
			{1,1,1,1,1}}
	};
private:
	
	PlantGenerator()=delete;
	~PlantGenerator()=delete;
};
