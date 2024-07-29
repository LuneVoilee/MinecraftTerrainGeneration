#pragma once
#include <unordered_map>

#include "MinecraftDemo/Meshes/Chunk.h"
DECLARE_LOG_CATEGORY_EXTERN(LogBuilding, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogArea, Log, All);

struct Point {
	int32 x;
	int32 y;
	Point(int ix=0,int iy=0):x(ix),y(iy){};
	Point(const Point& other):x(other.x),y(other.y){};
	Point(Point&& other):x(other.x),y(other.y){};
	
	friend uint32 GetTypeHash(const Point& other) {
		return HashCombine(other.x,other.y);
	};
	
	bool operator==(const Point& other) const {
		if(other.x==x && other.y==y) {
			return true;
		}
		return false;
	};
	bool operator==(Point& other) const {
		return operator==(const_cast<const Point&>(other));
	};
	
	const Point& operator=(const Point& rhs) {
		if(this != &rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
		}
		return *this;
	 }
	int32 operator[] (int32 index) const {
		if(index == 0) return x;
		if(index == 1) return y;
		return 0;
	};
};

class BuildingGenerator {
public:
	static void GenerateBuilding(TMap<uint64,int32>& HeightMap,Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay,TArray<TTuple<uint64,int32,int32>>& BuildingReadyToDisplay);
		BuildingGenerator()=delete;
		~BuildingGenerator()=delete;
private:
	static void DisplayPath(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay);
	static int32 getHeight(TMap<uint64,int32>& HeightMap,const Chunk& TheChunk, int32 x, int32 y);
	static void setHeight(TMap<uint64,int32>& HeightMap,const Chunk& TheChunk, int32 x, int32 y,int32 value);
	static void GenerateBuildingArea(TMap<uint64,int32>& HeightMap,Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay);
	static void DisplayBuildings( TMap<uint64,int32>& HeightMap,Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool, TMap<uint64, int32>&
	                             CubeReadyToDisplay, TArray<TTuple<uint64, int32, int32>>& BuildingReadyToDisplay);
	static bool TryToPlaceOneHouse(TMap<uint64,int32>& HeightMap,Chunk& TheChunk, int32 size0, int32 size1, int32 type, int32 rotate, Point point, TMap<
	                               uint64, int32>& CubeTypePool, TArray<TTuple<uint64, int32, int32>>& BuildingReadyToDisplay);
	static inline TSet<Point> Area={};
	static inline Point CapitalPoint = {};
	static inline TArray<FVector2d> BuildingPoint = {};
	static inline FVector2d globalOffset;
	
};

