#pragma once
#include <functional>
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
	}
	
	bool operator==(const Point& other) const {
		if(other.x==x && other.y==y) {
			return true;
		}
		return false;
	}
	bool operator==(Point& other) const {
		return operator==(const_cast<const Point&>(other));
	}
	
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
	}
};

class BuildingGenerator {
public:
	static void GenerateBuilding(TMap<uint64,int32>& HeightMap,Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay,TArray<TTuple<uint64,int32,int32>>& BuildingReadyToDisplay);
		BuildingGenerator()=delete;
		~BuildingGenerator()=delete;
private:
	static void DisplayPath(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay);
	static int32 getHeight(TMap<uint64, int32>& HeightMap, int32 gx, int32 gy);
	static bool setHeight(TMap<uint64, int32>& HeightMap, int32 gx, int32 gy, int32 value);
	static void GenerateBuildingArea(TMap<uint64,int32>& HeightMap,Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,TMap<uint64, int32>& CubeReadyToDisplay);
	static void DisplayBuildings( TMap<uint64,int32>& HeightMap,Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool, TMap<uint64, int32>&
	                             CubeReadyToDisplay, TArray<TTuple<uint64, int32, int32>>& BuildingReadyToDisplay);
	static bool TryToPlaceOneHouse(TMap<uint64, int32>& HeightMap, int32 size0, int32 size1, int32 type, int32 rotate, Point point, TMap<uint64, int32>&
	                               CubeTypePool, TArray<TTuple<uint64, int32, int32>>& BuildingReadyToDisplay);
	static inline TSet<Point> Area={};
	static inline Point CapitalPoint = {};
	static inline TArray<FVector2d> BuildingPoint = {};
	static inline FVector2d globalOffset;
	
};

// //open节点结构
// struct OpenNode;
//
// //比较器，用以优先队列的指针类型比较
// struct OpenPointPtrCompare;
//
// /**
//  * A*寻路器
//  */
// class TERRIANGENERATEDEMO_API PathFinder
// {
// public:
// 	//寻路
// 	static TArray<FVector2D> findPath(FVector2D startPos,FVector2D endPos);
// 	//设置寻路障碍条件
// 	static void setConditionInBarrier(std::function<bool(FVector2D pos)> func);
// 	//设置寻路权值公式
// 	static void setWeightFormula(std::function<TPair<float,float>(FVector2D pos,FVector2D endPos,float cost)> func);
// private:
// 	//创建一个开启点
// 	static OpenNode* createOpenNode(FVector2D startPos,FVector2D endPos, float cost,OpenNode* fatherNode);
// 	//开启检查，检查父节点
// 	static void open(OpenNode& pointToOpen,FVector2D endPos);
// private:
// 	static int depth;          //记录深度
// 	static int depthLimit;     //深度限制	
//
// 	//函数接口：检查是否有障碍物（不可走）
// 	static std::function<bool(FVector2D pos)> inBarrier;
// 	
// 	//函数接口：计算权值公式
// 	static std::function<TPair<float,float>(FVector2D pos,FVector2D endPos,float cost)> weightFormula;
//
// 	//使用最大优先队列
// 	static std::priority_queue<OpenNode*, std::vector<OpenNode*>, OpenPointPtrCompare> openlist;
//
// 	//存储OpenNode的内存空间
// 	static std::vector<OpenNode> pointList;
// private:
// 	PathFinder() = delete;
// 	~PathFinder() = delete;
// };
//
// struct OpenNode{
// 	FVector2D pos;           // 位置
// 	float cost;              // 耗费值
// 	float pred;              // 预测值
// 	OpenNode* father;        // 父节点
// 	OpenNode() = default;
// 	OpenNode(FVector2D pos,float cost,float pred, OpenNode* fatherNode)
// 		:pos(pos),cost(cost),pred(pred),father(fatherNode){}
// };
//
// struct OpenPointPtrCompare {
// 	bool operator()(OpenNode* a, OpenNode* b) {return a->pred > b->pred;}
// };