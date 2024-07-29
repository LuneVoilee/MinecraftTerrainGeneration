#pragma once
#include "MinecraftDemo/Meshes/Chunk.h"
DECLARE_LOG_CATEGORY_EXTERN(LogBuilding, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogArea, Log, All);
class BuildingGenerator {
	public:
		static inline TSet<uint64> Area={};
		static inline TArray<int32> CapitalPoint = {};
		static inline TArray<FVector2d> BuildingPoint = {};
		static void DisplayPath(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay);
		static void GenerateBuildingArea(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay);
		static void GenerateBuilding(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay,TArray<TTuple<uint64,int32,int32>>& BuildingReadyToDisplay);
		static void DisplayBuildings(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay,TArray<TTuple<uint64,int32,int32>>& BuildingReadyToDisplay);
		static bool TryToPlaceOneHouse(Chunk& TheChunk, int32 size0, int32 size1, int32 type, int32 rotate, TArray<int32> Point, TMap<uint64, int32>&
		                               CubeTypePool, TArray<TTuple<uint64, int32, int32>>& BuildingReadyToDisplay);
		BuildingGenerator()=delete;
		~BuildingGenerator()=delete;
	
};
